/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org
 *
 * Copyright: 2012 Razor team
 * Authors:
 *   Alec Moskvin <alecm@gmx.com>
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

#include "notifyd.h"

#include <QDebug>


/*
 * Implementation of class Notifyd
 */

Notifyd::Notifyd(QObject* parent)
    : QObject(parent),
      mId(0)
{
    m_area = new NotificationArea();
    m_settings = new LXQt::Settings("notifications");
    reloadSettings();

    connect(this, &Notifyd::notificationAdded,
            m_area->layout(), &NotificationLayout::addNotification);
    connect(this, &Notifyd::notificationClosed,
            m_area->layout(), &NotificationLayout::removeNotification);
    // feedback for original caller
    connect(m_area->layout(), &NotificationLayout::notificationClosed,
            this, &Notifyd::NotificationClosed);
    connect(m_area->layout(), &NotificationLayout::actionInvoked,
            this, &Notifyd::ActionInvoked);

    connect(m_settings, &LXQt::Settings::settingsChanged,
            this, &Notifyd::reloadSettings);

}

Notifyd::~Notifyd()
{
    m_area->deleteLater();
}

void Notifyd::CloseNotification(uint id)
{
    emit notificationClosed(id, 3);
}

QStringList Notifyd::GetCapabilities()
{
    QStringList caps;
    caps
         << "actions"
      // << "action-icons"
         << "body"
         << "body-hyperlinks"
         << "body-images"
         << "body-markup"
      // << "icon-multi"
      // << "icon-static"
         << "persistence"
      // << "sound"
      ;
    return caps;
}

QString Notifyd::GetServerInformation(QString& vendor,
                                      QString& version,
                                      QString& spec_version)
{
    spec_version = QString("1.2");
    version = QString(LXQT_VERSION);
    vendor = QString("lxqt.org");
    return QString("lxqt-notificationd");
}

uint Notifyd::Notify(const QString& app_name,
                     uint replaces_id,
                     const QString& app_icon,
                     const QString& summary,
                     const QString& body,
                     const QStringList& actions,
                     const QVariantMap& hints,
                     int expire_timeout
                     )
{
    uint ret;
    if (replaces_id == 0)
    {
        mId++;
        ret = mId;
    }
    else
        ret = replaces_id;
#if 0
    qDebug() << QString("Notify(\n"
                        "  app_name = %1\n"
                        "  replaces_id = %2\n"
                        "  app_icon = %3\n"
                        "  summary = %4\n"
                        "  body = %5\n"
                        ).arg(app_name, QString::number(replaces_id), app_icon, summary, body)
                     << "  actions =" << actions << endl
                     << "  hints =" << hints << endl
             << QString("  expire_timeout = %1\n) => %2").arg(QString::number(expire_timeout), QString::number(mId));
#endif

    // handling the "server decides" timeout
    if (expire_timeout == -1) {
        expire_timeout = m_serverTimeout;
        expire_timeout *= 1000;
    }

    emit notificationAdded(ret, app_name, summary, body, app_icon, expire_timeout, actions, hints);

    return ret;
}

void Notifyd::reloadSettings()
{
    m_serverTimeout = m_settings->value("server_decides", 10).toInt();
    m_area->setSettings(
            m_settings->value("placement", "bottom-right").toString().toLower(),
            m_settings->value("width", 300).toInt(),
            m_settings->value("spacing", 6).toInt());
}
