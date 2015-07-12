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

#ifndef NEBULAETRANSLATION_H
#define NEBULAETRANSLATION_H

#include <QtCore/QLocale>
#include <QtCore/QTranslator>
#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>

#include <QtDebug>

inline void NebulaeTranslateApplication()
{
    QTranslator t;
    bool ok = t.load("translation_" + QLocale::system().name().split('_').first(), "/usr/share/nebulae/" + qApp->applicationName() + "/translations/");
    if(ok)
        qApp->installTranslator(&t);
    else
        qDebug() << "Error while loading " << "/usr/share/nebulae/" + qApp->applicationName() + "translations/translation_" + QLocale::system().name().split('_').first();
}

#endif // NEBULAETRANSLATION_H
