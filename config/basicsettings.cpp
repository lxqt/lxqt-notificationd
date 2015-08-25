/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2010-2011 Razor team
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

#include <LXQt/Notification>

#include "basicsettings.h"
#include "mainwindow.h"


BasicSettings::BasicSettings(LXQt::Settings* settings, QWidget *parent) :
    QWidget(parent),
    mSettings(settings)
{
    setupUi(this);

    restoreSettings();

    connect(topLeftRB,      SIGNAL(clicked()), this, SLOT(updateNotification()));
    connect(topCenterRB,    SIGNAL(clicked()), this, SLOT(updateNotification()));
    connect(topRightRB,     SIGNAL(clicked()), this, SLOT(updateNotification()));
    connect(centerLeftRB,   SIGNAL(clicked()), this, SLOT(updateNotification()));
    connect(centerCenterRB, SIGNAL(clicked()), this, SLOT(updateNotification()));
    connect(centerRightRB,  SIGNAL(clicked()), this, SLOT(updateNotification()));
    connect(bottomLeftRB,   SIGNAL(clicked()), this, SLOT(updateNotification()));
    connect(bottomCenterRB, SIGNAL(clicked()), this, SLOT(updateNotification()));
    connect(bottomRightRB,  SIGNAL(clicked()), this, SLOT(updateNotification()));

    LXQt::Notification serverTest;
    QString serverName = serverTest.serverInfo().name;
    if (serverName != "lxqt-notificationd")
    {
        if (serverName.isEmpty())
            warningLabel->setText(tr("<b>Warning:</b> No notifications daemon is running.\n"
            "A fallback will be used."));
        else
            warningLabel->setText(tr("<b>Warning:</b> A third-party notifications daemon (%1) is running.\n"
            "These settings won't have any effect on it!").arg(serverName));
    }
}

BasicSettings::~BasicSettings()
{
}

void BasicSettings::restoreSettings()
{
    QString placement = mSettings->value(QStringLiteral("placement"),
                                         QStringLiteral("bottom-right")).toString().toLower();

    if (QStringLiteral("top-left") == placement)
        topLeftRB->setChecked(true);
    else if (QStringLiteral("top-center") == placement)
        topCenterRB->setChecked(true);
    else if (QStringLiteral("top-right") == placement)
        topRightRB->setChecked(true);
    else if (QStringLiteral("center-left") == placement)
        centerLeftRB->setChecked(true);
    else if (QStringLiteral("center-center") == placement)
        centerCenterRB->setChecked(true);
    else if (QStringLiteral("center-right") == placement)
        centerRightRB->setChecked(true);
    else if (QStringLiteral("bottom-left") == placement)
        bottomLeftRB->setChecked(true);
    else if (QStringLiteral("bottom-center") == placement)
        bottomCenterRB->setChecked(true);
    else if (QStringLiteral("bottom-right") == placement)
        bottomRightRB->setChecked(true);
}

void BasicSettings::updateNotification()
{
    QString align;
    if (topLeftRB->isChecked())
        align = QStringLiteral("top-left");
    else if (topCenterRB->isChecked())
        align = QStringLiteral("top-center");
    else if (topRightRB->isChecked())
        align = QStringLiteral("top-right");
    else if (centerLeftRB->isChecked())
        align = QStringLiteral("center-left");
    else if (centerCenterRB->isChecked())
        align = QStringLiteral("center-center");
    else if (centerRightRB->isChecked())
        align = QStringLiteral("center-right");
    else if (bottomLeftRB->isChecked())
        align = QStringLiteral("bottom-left");
    else if (bottomCenterRB->isChecked())
        align = QStringLiteral("bottom-center");
    else // if (bottomRightRB->isChecked())
        align = QStringLiteral("bottom-right");

    mSettings->setValue(QStringLiteral("placement"), align);
    LXQt::Notification::notify(tr("Notification demo ") + align,
                               tr("This is a test notification.\n All notifications will now appear here on LXQt."),
                               QStringLiteral("lxqt-logo.png"));
}
