/*
 * This file is part of Nebulae.
 * Copyright (C) 2011 - Leo Testard <leo.testard@gmail.com>
 *
 * Nebulae is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nebulae is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Nebulae. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INFOMANAGER_H
#define INFOMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QHash>

// this class is used to store all the informations collected
// by the various steps of the installer
class InfoManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString locale READ locale WRITE setLocale)

public:
    InfoManager(QObject *parent = 0);

    // accessors for setting/reading properties
    void setLocale(QString);
    void setTimezone(QString);
    QString locale();
    QString timezone();

    // for debugging use
    void displayDebugInfo();

public slots:

private:
    // all string types informations
    QString m_locale;       // the full locale of the system, such as fr_EN or en_GB
    QString m_charsert;     // the charset for the locale, e.g. UTF-8
    QString m_timezone;         // the timezone, like "2" for GMT +2
    QString m_kblayout;     // the keyboard layout

    // informations about partitionning
    QHash<QString,QString> *m_toformat;     // paritions to format under the form <id,filesystem>, e.g. <sda1,ext4>
    QHash<QString,QString> *m_mounting;     // mounting points  under the form <id,point> like <sda1,/home>


};

#endif // INFOMANAGER_H
