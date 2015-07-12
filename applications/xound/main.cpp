/*Copyright (C) 2010 Revest Florent <florent.revest666@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "QtSingleApplication"
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include "include/mainwindow.h"

int main(int argc, char *argv[])  //Simply the start of the program, verify if Xound isn't already launched and install the translation!
{
    QtSingleApplication a(argc, argv);
    if(a.isRunning())
    {
        a.sendMessage(a.arguments().at(0));
        return 0;
    }
    else
    {
        a.setApplicationName(QString("Xound"));

        QTranslator *translator = new QTranslator();
        translator->load(QString("Xound_") + QLocale::system().name().section('_', 0, 0));
        a.installTranslator(translator);

        MainWindow w;
        w.show();
        a.setActivationWindow(&w);
        return a.exec();
    }
}
