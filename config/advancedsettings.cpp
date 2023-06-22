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

#include <QFileDialog>

#include "advancedsettings.h"
#include "mainwindow.h"


AdvancedSettings::AdvancedSettings(LXQt::Settings* settings, QWidget *parent):
    QWidget(parent),
    mSettings(settings)
{
    setupUi(this);
    restoreSettings();

    connect(serverDecidesBox, &QAbstractSpinBox::editingFinished, this, &AdvancedSettings::save);
    connect(unattendedBox, &QAbstractSpinBox::editingFinished, this, &AdvancedSettings::save);
    connect(blackListEdit, &QLineEdit::editingFinished, this, &AdvancedSettings::save);
    connect(doNotDisturbBtn, &QCheckBox::clicked, this, &AdvancedSettings::save);
    connect(screenWithMouseBtn, &QCheckBox::clicked, this, &AdvancedSettings::save);
}

AdvancedSettings::~AdvancedSettings() = default;

void AdvancedSettings::restoreSettings()
{
    int serverDecides = mSettings->value(QL1S("server_decides"), 10).toInt();
    if (serverDecides <= 0)
        serverDecides = 10;
    serverDecidesBox->setValue(serverDecides);

    unattendedBox->setValue(mSettings->value(QL1S("unattendedMaxNum"), 10).toInt());
    blackListEdit->setText(mSettings->value(QL1S("blackList")).toStringList().join (QL1S(",")));
    doNotDisturbBtn->setChecked(mSettings->value(QL1S("doNotDisturb"), false).toBool());

    // TODO: it would be nice to put more options here such as:
    // fixed screen to display notification
    // notification shows in all screens (is it worthy the increased ram usage?)
    screenWithMouseBtn->setChecked(mSettings->value(QL1S("screenWithMouse"), false).toBool());
}

void AdvancedSettings::save()
{
    mSettings->setValue(QL1S("server_decides"), serverDecidesBox->value());
    mSettings->setValue(QL1S("unattendedMaxNum"), unattendedBox->value());
    mSettings->setValue(QL1S("doNotDisturb"), doNotDisturbBtn->isChecked());

    mSettings->setValue(QL1S("screenWithMouse"), screenWithMouseBtn->isChecked());

    QString blackList = blackListEdit->text();
    if (!blackList.isEmpty())
    {
        QStringList l = blackList.split(QL1S(","), Qt::SkipEmptyParts);
        l.removeDuplicates();
        mSettings->setValue(QL1S("blackList"), l);
    }
    else
        mSettings->remove(QL1S("blackList"));
}
