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

#include "infomanager.h"

#include <QtDebug>

InfoManager::InfoManager(QObject *parent) : QObject(parent)
{
}

void InfoManager::setLocale(QString locale)
{
    m_locale = locale;
}

void InfoManager::setTimezone(QString timezone)
{
    m_timezone = timezone;
}

QString InfoManager::locale()
{
    return m_locale;
}

QString InfoManager::timezone()
{
    return m_timezone;
}

void InfoManager::displayDebugInfo()
{
    qDebug() << "Locale: " << m_locale;
    qDebug() << "Timezone: " << m_timezone;
}
