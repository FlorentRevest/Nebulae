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

#include <QtCore/QObject>
#include <QtCore/QLocale>
#include <QtCore/QTranslator>

#include "nebulae-application.h"
#include "nebulae-translation.h"

#include "mainwindow.h"

#include <QtDebug>

int main(int argc, char *argv[])
{
    NebulaeApplication a(argc, argv, "admin-panel");

    /*
    QTranslator t;
    QString name = "admin-panel";
    QString translation = "/usr/share/nebulae/" + name + "/translations/translation_" + QLocale::system().name().split('_').first();
    bool ok = t.load(translation);
    qDebug() << translation << ok;
    a.installTranslator(&t);
    //*/
    NebulaeTranslateApplication();

    MainWindow w;
    w.show();

    return a.exec();
}
