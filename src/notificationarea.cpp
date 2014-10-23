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

    int h = workArea.height();
    int safeHeight = contentHeight > h ? h : contentHeight;
    int x, y;

    if (m_placement == "bottom-right")
    {
        x = workArea.right() - width() - m_spacing;
        y = workArea.bottom() - safeHeight  - m_spacing;
    }
    else if (m_placement == "bottom-left")
    {
        x = workArea.x() + m_spacing;
        y = workArea.bottom() - safeHeight - m_spacing;
    }
    else if (m_placement == "top-right")
    {
        x = workArea.right() - width() - m_spacing;
        y = workArea.y() + m_spacing;
    }
    else if (m_placement == "top-left")
    {
        x = workArea.x() + m_spacing;
        y = workArea.y() + m_spacing;
    }
    else
    {
        x = workArea.right() - width() - m_spacing;
        y = workArea.bottom() - safeHeight;
    }

    move(x, y);
    resize(width(), safeHeight);
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
