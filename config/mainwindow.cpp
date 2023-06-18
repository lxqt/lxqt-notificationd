/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org
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

#include <QtCore/QProcess>
#include <QMessageBox>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <LXQt/Globals>
#include <LXQt/ConfigDialog>

#include "mainwindow.h"
#include "generalsettings.h"
#include "appearancesettings.h"


MainWindow::MainWindow(QWidget *parent) :
    LXQt::ConfigDialog(tr("Desktop Notifications"), new LXQt::Settings(QSL("notifications")), parent)
{
    GeneralSettings* menu = new GeneralSettings(mSettings, this);
    addPage(menu, tr("General Settings"), QSL("preferences-desktop-notification-bell"));
    connect(this, &MainWindow::reset, menu, &GeneralSettings::restoreSettings);

    AppearanceSettings* Appearance = new AppearanceSettings(mSettings, this);
    addPage(Appearance, tr("Position"), QSL("preferences-desktop-notification"));
    connect(this, &MainWindow::reset, Appearance, &AppearanceSettings::restoreSettings);
}

MainWindow::~MainWindow()
{
    delete mSettings;
}
