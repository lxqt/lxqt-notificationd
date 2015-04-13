/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
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

#include <QDesktopWidget>
#include <QApplication>
#include <LXQt/Settings>
#include "notificationarea.h"


NotificationArea::NotificationArea(QWidget *parent)
    : QScrollArea(parent),
      m_spacing(-1)
{
    setObjectName("NotificationArea");

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

    connect(m_layout, SIGNAL(allNotificationsClosed()), this, SLOT(close()));
    connect(m_layout, SIGNAL(notificationAvailable()), this, SLOT(show()));
    connect(m_layout, SIGNAL(heightChanged(int)), this, SLOT(setHeight(int)));
    connect(qApp->desktop(), SIGNAL(workAreaResized(int)), SLOT(setHeight()));
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
    QRect workArea = qApp->desktop()->availableGeometry(qApp->desktop()->primaryScreen());

    workArea -= QMargins(m_spacing, m_spacing, m_spacing, m_spacing);
    QRect notif_rect = workArea.normalized();
    notif_rect.setWidth(width());
    if (notif_rect.height() > contentHeight)
        notif_rect.setHeight(contentHeight);

    // no move needed for "top-left"
    if ("top-center" == m_placement)
    {
        notif_rect.moveCenter(workArea.center());
        notif_rect.moveTop(workArea.top());
    } else if ("top-right" == m_placement)
    {
        notif_rect.moveRight(workArea.right());
    } else if ("center-left" == m_placement)
    {
        notif_rect.moveCenter(workArea.center());
        notif_rect.moveLeft(workArea.left());
    } else if ("center-center" == m_placement)
    {
        notif_rect.moveCenter(workArea.center());
    } else if ("center-right" == m_placement)
    {
        notif_rect.moveCenter(workArea.center());
        notif_rect.moveRight(workArea.right());
    } else if ("bottom-left" == m_placement)
    {
        notif_rect.moveBottom(workArea.bottom());
    } else if ("bottom-center" == m_placement)
    {
        notif_rect.moveCenter(workArea.center());
        notif_rect.moveBottom(workArea.bottom());
    } else if ("bottom-right" == m_placement)
    {
        notif_rect.moveBottomRight(workArea.bottomRight());
    }

    setGeometry(notif_rect);
    // always show the latest notification
    ensureVisible(0, contentHeight, 0, 0);
}

void NotificationArea::setSettings(const QString &placement, int width, int spacing)
{
    m_placement = placement;

    setMaximumWidth(width);
    setMinimumWidth(width);

    m_spacing = spacing;
    m_layout->setSizes(m_spacing, width);

    this->setHeight(widget()->height());
}
