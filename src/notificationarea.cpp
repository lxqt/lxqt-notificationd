/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org
 *
 * Copyright: 2012 Razor team
 * Authors:
 *   Petr Vanek <petr@scribus.info>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include <QApplication>
#include <QScreen>
#include <QWindow>
#include <LXQt/Globals>
#include <LXQt/Settings>
#include "notificationarea.h"
#include <LayerShellQt/Shell>
#include <LayerShellQt/Window>

NotificationArea::NotificationArea(QWidget *parent)
    : QScrollArea(parent),
      m_spacing(-1),
      m_screenWithMouse(false)
{
    setObjectName(QSL("NotificationArea"));

    setWindowFlags(Qt::X11BypassWindowManagerHint
                   | Qt::FramelessWindowHint
                   | Qt::WindowStaysOnTopHint);
    // Hack to ensure the fully transparent QGraphicsView background
    QPalette palette;
    palette.setBrush(QPalette::Base, Qt::NoBrush);
    setPalette(palette);
    // Required to display wallpaper
    setAttribute(Qt::WA_TranslucentBackground);
    // no border at all finally
    setFrameShape(QFrame::NoFrame);

    m_layout = new NotificationLayout(this);
    setWidget(m_layout);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(m_layout, &NotificationLayout::allNotificationsClosed, this, [this] {
        if (QGuiApplication::platformName() == QStringLiteral("wayland"))
            hide(); // if it is closed, the shell properties will not be effective the next time
        else
            close();
    });
    connect(m_layout, &NotificationLayout::notificationAvailable, this, [this] {
        setLayerShell();
        show();
    });
    connect(m_layout, &NotificationLayout::heightChanged, this, &NotificationArea::setHeight);

    connect(qApp, &QGuiApplication::screenAdded, this, [this] (QScreen* newScreen) {
        connect(newScreen, &QScreen::availableGeometryChanged, this, &NotificationArea::availableGeometryChanged);
    });
    connect(qApp, &QGuiApplication::screenRemoved, this, [this] (QScreen* oldScreen) {
        // do not wait until the screen is really removed
        disconnect(oldScreen, &QScreen::availableGeometryChanged, this, &NotificationArea::availableGeometryChanged);
    });
    const auto screens = qApp->screens();
    for (const auto& screen : screens)
    {
        connect(screen, &QScreen::availableGeometryChanged, this, &NotificationArea::availableGeometryChanged);
    }
}

void NotificationArea::availableGeometryChanged(const QRect& /*availableGeometry*/)
{
    // adjust geometry if the available geometry changes while a notification is visible
    if (isVisible())
        setHeight();
}

void NotificationArea::setHeight(int contentHeight)
{
    if (contentHeight == 0)
    {
        resize(width(), 0);
        hide();
        return;
    }

    QScreen *widgetScreen = nullptr;
    if (m_screenWithMouse)
    {
        if (isVisible() && contentHeight == -1) // called by availableGeometryChanged()
        {
            if (QWindow *win = windowHandle())
                widgetScreen = win->screen();
        }
        if (widgetScreen == nullptr)
            widgetScreen = QGuiApplication::screenAt(QCursor::pos());
    }

    if (widgetScreen == nullptr)
        widgetScreen = qApp->primaryScreen();

    if (contentHeight == -1)
        contentHeight = height();

    QRect workArea = widgetScreen->availableGeometry();
    workArea -= QMargins(m_spacing, m_spacing, m_spacing, m_spacing);
    QRect notif_rect = workArea.normalized();
    notif_rect.setWidth(width());
    if (notif_rect.height() > contentHeight)
        notif_rect.setHeight(contentHeight);

    if (QGuiApplication::platformName() == QStringLiteral("wayland"))
    {
        resize(notif_rect.size());
    }
    else
    {
        // no move needed for "top-left"
        if (QL1S("top-center") == m_placement)
        {
            notif_rect.moveCenter(workArea.center());
            notif_rect.moveTop(workArea.top());
        } else if (QL1S("top-right") == m_placement)
        {
            notif_rect.moveRight(workArea.right());
        } else if (QL1S("center-left") == m_placement)
        {
            notif_rect.moveCenter(workArea.center());
            notif_rect.moveLeft(workArea.left());
        } else if (QL1S("center-center") == m_placement)
        {
            notif_rect.moveCenter(workArea.center());
        } else if (QL1S("center-right") == m_placement)
        {
            notif_rect.moveCenter(workArea.center());
            notif_rect.moveRight(workArea.right());
        } else if (QL1S("bottom-left") == m_placement)
        {
            notif_rect.moveBottom(workArea.bottom());
        } else if (QL1S("bottom-center") == m_placement)
        {
            notif_rect.moveCenter(workArea.center());
            notif_rect.moveBottom(workArea.bottom());
        } else if (QL1S("bottom-right") == m_placement)
        {
            notif_rect.moveBottomRight(workArea.bottomRight());
        }

        setGeometry(notif_rect);
    }

    // always show the latest notification
    ensureVisible(0, contentHeight, 0, 0);
}

void NotificationArea::setSettings(const QString &placement, int width, int spacing, int unattendedMaxNum, bool screenWithMouse, const QStringList &blackList) {
    m_placement = placement;

    setMaximumWidth(width);
    setMinimumWidth(width);

    m_spacing = spacing;
    m_layout->setSizes(m_spacing, width);

    m_screenWithMouse = screenWithMouse;

    this->setHeight(widget()->height());

    m_layout->setUnattendedMaxNum(unattendedMaxNum);
    m_layout->setBlackList(blackList);
}

void NotificationArea::setLayerShell()
{
    if (!isVisible() && QGuiApplication::platformName() == QStringLiteral("wayland"))
    {
        winId();
        if (QWindow* win = windowHandle())
        {
            if (LayerShellQt::Window* layershell = LayerShellQt::Window::get(win))
            {
                layershell->setLayer(LayerShellQt::Window::Layer::LayerOverlay);
                layershell->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);
                layershell->setMargins(QMargins(m_spacing, m_spacing, m_spacing, m_spacing));
                layershell->setScope(QStringLiteral("notification"));
                LayerShellQt::Window::Anchors anchors;
                if (QL1S("top-center") == m_placement)
                {
                    anchors = {LayerShellQt::Window::AnchorTop};
                }
                else if (QL1S("top-left") == m_placement)
                {
                    anchors = {LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorLeft};
                }
                else if (QL1S("top-right") == m_placement)
                {
                    anchors = {LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorRight};
                }
                else if (QL1S("center-left") == m_placement)
                {
                    anchors = {LayerShellQt::Window::AnchorLeft};
                }
                else if (QL1S("center-right") == m_placement)
                {
                    anchors = {LayerShellQt::Window::AnchorRight};
                }
                else if (QL1S("bottom-left") == m_placement)
                {
                    anchors = {LayerShellQt::Window::AnchorLeft | LayerShellQt::Window::AnchorBottom};
                }
                else if (QL1S("bottom-center") == m_placement)
                {
                    anchors = {LayerShellQt::Window::AnchorBottom};
                }
                else if (QL1S("bottom-right") == m_placement)
                {
                    anchors = {LayerShellQt::Window::AnchorBottom | LayerShellQt::Window::AnchorRight};
                }
                layershell->setAnchors(anchors);
            }
        }
    }
}
