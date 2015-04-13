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


BasicSettings::BasicSettings(LxQt::Settings* settings, QWidget *parent) :
    QWidget(parent),
    mSettings(settings)
{
    setupUi(this);

    restoreSettings();

    connect(topButton, SIGNAL(clicked()), this, SLOT(updateNotification()));
    connect(verCenterButton, SIGNAL(clicked()), this, SLOT(updateNotification()));
    connect(bottomButton, SIGNAL(clicked()), this, SLOT(updateNotification()));
    connect(leftButton, SIGNAL(clicked()), this, SLOT(updateNotification()));
    connect(horCenterButton, SIGNAL(clicked()), this, SLOT(updateNotification()));
    connect(rightButton, SIGNAL(clicked()), this, SLOT(updateNotification()));

    LxQt::Notification serverTest;
    QString serverName = serverTest.serverInfo().name;
    if (serverName != "lxqt-notificationd")
    {
        if (serverName.isEmpty())
        {
            warningLabel->setText(tr(
                "<b>Warning:</b> No notifications daemon is running.\n"
                "A fallback will be used."));
        }
        else
        {
            warningLabel->setText(tr(
                "<b>Warning:</b> A third-party notifications daemon (%1) is running.\n"
                "These settings won't have any effect on it!"
                ).arg(serverName));
        }
    }
}

BasicSettings::~BasicSettings()
{
}

void BasicSettings::restoreSettings()
{
    QString placement = mSettings->value("placement", "bottom-right").toString().toLower();

    if ("top-left" == placement)
    {
        topButton->setChecked(true);
        leftButton->setChecked(true);
    } else if ("top-center" == placement)
    {
        topButton->setChecked(true);
        horCenterButton->setChecked(true);
    } else if ("top-right" == placement)
    {
        topButton->setChecked(true);
        rightButton->setChecked(true);
    } else if ("center-left" == placement)
    {
        verCenterButton->setChecked(true);
        leftButton->setChecked(true);
    } else if ("center-center" == placement)
    {
        verCenterButton->setChecked(true);
        horCenterButton->setChecked(true);
    }
    if ("center-right" == placement)
    {
        verCenterButton->setChecked(true);
        rightButton->setChecked(true);
    } else if ("bottom-left" == placement)
    {
        bottomButton->setChecked(true);
        leftButton->setChecked(true);
    } else if ("bottom-center" == placement)
    {
        bottomButton->setChecked(true);
        horCenterButton->setChecked(true);
    }
    if ("bottom-right" == placement)
    {
        bottomButton->setChecked(true);
        rightButton->setChecked(true);
    }
}

void BasicSettings::updateNotification()
{
    QString align(topButton->isChecked() ? "top" : (verCenterButton->isChecked() ? "center" : "bottom"));
    align += '-';
    align += (leftButton->isChecked() ? "left" : (horCenterButton->isChecked() ? "center" : "right"));
    mSettings->setValue("placement", align);

    LxQt::Notification::notify(//"lxqt-config-notificationd",
                              tr("Notification demo"),
                              tr("This is a test notification.\n"
                                 "All notifications will now appear here on LXQt."),
                              "lxqt-logo.png");
}
