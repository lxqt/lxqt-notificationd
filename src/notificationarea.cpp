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
#include <LXQt/Globals>
#include <LXQt/Settings>
#include "notificationarea.h"


NotificationArea::NotificationArea(QWidget *parent)
    : QScrollArea(parent),
      m_spacing(-1),
      m_screenWithMouse(false),
      m_workScreen(nullptr)
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

    connect(m_layout, &NotificationLayout::allNotificationsClosed, this, &NotificationArea::close);
    connect(m_layout, &NotificationLayout::notificationAvailable, this, &NotificationArea::show);
    connect(m_layout, &NotificationLayout::heightChanged, this, &NotificationArea::setHeight);
    connect(qApp, &QGuiApplication::primaryScreenChanged, this, &NotificationArea::primaryScreenChanged);
    connect(qApp, &QGuiApplication::screenAdded, this, &NotificationArea::screenAdded);
    connect(qApp, &QGuiApplication::screenRemoved, this, &NotificationArea::screenRemoved);

    connect(qApp->primaryScreen(), &QScreen::availableGeometryChanged, this, &NotificationArea::availableGeometryChanged);
    m_screensList.append(qApp->primaryScreen());

    updateWorkScreen();
}

void NotificationArea::screenAdded(QScreen *screen)
{
    if (!m_screensList.contains(screen))
    {
        connect(screen, &QScreen::availableGeometryChanged, this, &NotificationArea::availableGeometryChanged);
        m_screensList.append(screen);
    }
}

void NotificationArea::screenRemoved(QScreen *screen)
{
    if (m_screensList.contains(screen))
        m_screensList.removeOne(screen); // Qt will disconnect the slot by itself so there's no need for a disconnect call here

     // after removing it from screensList we can call updateWorkArea(), at this point Qt didnt deleted the screen from QGuiApplication::screens() so we can't use QGuiApplication::screens() in updateWorkArea()
     if (m_workScreen == screen)
         updateWorkScreen();
}

void NotificationArea::primaryScreenChanged(QScreen *screen)
{
    Q_UNUSED(screen);
    if (!m_screenWithMouse)
        updateWorkScreen();
}

void NotificationArea::availableGeometryChanged(const QRect& geometry)
{
    // if its not visible then theres no need to change its height now (it changes everytime a notification is added/closed)
    if (isVisible() && m_workScreen->geometry().contains(geometry) )
        setHeight(-1);
}
void NotificationArea::updateWorkScreen()
{
    if (m_screenWithMouse)
    {
        bool screenWasFound = false;
        for (const auto &screen: m_screensList)
        {
            if (screen->geometry().contains(QCursor::pos()))
            {
                m_workScreen = screen;
                screenWasFound = true;
                break;
            }
        }
        if (!screenWasFound) // there's a reported bug for a few WM where the mouse goes out of screen geometry, this avoids the notification area to get lost
            m_workScreen = qApp->primaryScreen();
    } else
        m_workScreen= qApp->primaryScreen();
}
void NotificationArea::setHeight(int contentHeight)
{
    if (contentHeight == 0)
    {
        resize(width(), 0);
        hide();
        return;
    }

    if (contentHeight == -1)
        contentHeight = height();

    // FIXME: Qt does not seem to update QDesktopWidget::primaryScreen().
    // After we change the primary screen with xrandr, Qt still returns the same value.
    // I think it's a bug of Qt.

    // avoids unnecessary updates in workArea
    if (!m_workScreen || (m_screenWithMouse && !m_workScreen->geometry().contains(QCursor::pos())) || (!m_screenWithMouse && (qApp->primaryScreen()!=m_workScreen)))
    {
        updateWorkScreen();
    }

    QRect workArea = m_workScreen->availableGeometry();
    workArea -= QMargins(m_spacing, m_spacing, m_spacing, m_spacing);
    QRect notif_rect = workArea.normalized();
    notif_rect.setWidth(width());
    if (notif_rect.height() > contentHeight)
        notif_rect.setHeight(contentHeight);

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
    // always show the latest notification
    ensureVisible(0, contentHeight, 0, 0);
}

void NotificationArea::setSettings(const QString &placement, int width, int spacing, int unattendedMaxNum, bool screenWithMouse, const QStringList &blackList) {
    m_placement = placement;

    setMaximumWidth(width);
    setMinimumWidth(width);

    m_spacing = spacing;
    m_layout->setSizes(m_spacing, width);

    if ((m_screenWithMouse != screenWithMouse))
    {
        m_screenWithMouse = screenWithMouse;
        const auto screens = qApp->screens();
        for (const auto &screen: screens)
        {
            if (screen == qApp->primaryScreen())
                continue;

            if (m_screenWithMouse)
            {
                if (!m_screensList.contains(screen)) {
                    connect(screen, &QScreen::availableGeometryChanged, this, &NotificationArea::availableGeometryChanged);
                    m_screensList.append(screen);
                }
            }
            else
            {
                if (m_screensList.contains(screen))
                {
                    disconnect(screen, &QScreen::availableGeometryChanged, this, &NotificationArea::availableGeometryChanged);
                    m_screensList.removeOne(screen);
                }
            }
        }

    }

    this->setHeight(widget()->height());

    m_layout->setUnattendedMaxNum(unattendedMaxNum);
    m_layout->setBlackList(blackList);
}
