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

#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QProcess>
#include <QtDebug>

#include <nebulae/nebulae-desktopentry.h>
#include <nebulae/nebulae-basedir.h>

// returns true if can be found in one of the folder of the PATH, or returns false
bool isInPath(const QString &binary)
{
    foreach(QString dir, PATH)
    {
        if(QDir(dir).entryList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot).contains(binary, Qt::CaseSensitive))
            return true;
    }

    // not found in any folder
    return false;
}

// returns true if "path" points to a valid binary, or is the name of a binary in the PATH, otherwise returns false
bool validBinaryPath(const QString &binary)
{
    if(QFileInfo(binary).isAbsolute() && QFileInfo(binary).exists())
        return true;
    else if(isInPath(binary))
        return true;
    else
        return false;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    bool simul = false;
    if(a.arguments().contains("--simulation"))
        simul = true;

    qDebug() << " = Nebulae Autostart Manager launched = " + QDateTime::currentDateTime().toString("MM-dd-yyyy hh:mm") << " =\n";

    // will list the file name we can not launch //
    QStringList forbidden_files;

    // launch files from the home directory, according to Freedesktop : $XDG_CONFIG_HOME/autostart //
    if(QDir(XDG_CONFIG_HOME + "/autostart/").exists())
    {
        qDebug() << "Entering " << XDG_CONFIG_HOME + "/autostart/";
        // list the content of the directory
        foreach(QFileInfo file, QDir(XDG_CONFIG_HOME + "/autostart/").entryInfoList(QStringList() << "*.desktop", QDir::Files | QDir::NoDotAndDotDot))
        {
            if(file.exists() && file.isReadable())
            {
                DesktopEntry d(file.absoluteFilePath());

                // if a file with this name was hiodden in a more prioritary directory, we have to ignore it
                if(forbidden_files.contains(file.fileName()))
                    continue;

                // check we can display it
                if(!d.onlyShowIn().isEmpty() && !d.onlyShowIn().contains("X-NEBULAE")) {
                    qDebug() << " * " << file.fileName() << " cannot be launched by Nebulae because of its OnlyShowIn key";
                    continue;
                }
                if(!d.notShowIn().isEmpty() && d.notShowIn().contains("X-NEBULAE")){
                    qDebug() << " * " << file.fileName() << " cannot be launched by Nebulae because of its NotShowIn key";
                    continue;
                }

                // is it hidden ?
                if(d.hidden()) {
                    // yes, we can not display any file with its name
                    forbidden_files.append(file.fileName());
                    qDebug() << " * " << file.fileName() << " : Hidden file. Ignored.";
                }

                QString tryExec = d.tryExec();
                QString exec = d.exec();
                if((tryExec.isEmpty() || validBinaryPath(tryExec.split(' ').first())) && validBinaryPath(exec.split(' ').first())) {
                   QProcess p;
                   p.setWorkingDirectory(d.path().isEmpty() ? a.applicationDirPath() : d.path());
                   bool success = simul ? true : p.startDetached(exec.remove("%u", Qt::CaseInsensitive));
                   if(success)
                       qDebug() << "Starting " << file.fileName() << "... [ok]";
                   else
                       qDebug() << "Starting " << file.fileName() << "... [failed]";
                }
                else
                    qDebug() << "Starting " << file.fileName() << "... [failed] (bad tryExec or exec key)";
            }
        }
    }

    // same with each global directory //
    foreach(QString d, XDG_CONFIG_DIRS)
    {
        if(QDir(d + "/autostart/").exists())
        {
            qDebug() << "Entering " << d + "/autostart/";
            // list the content of the directory
            foreach(QFileInfo file, QDir(d + "/autostart/").entryInfoList(QStringList() << "*.desktop", QDir::Files | QDir::NoDotAndDotDot))
            {
                if(file.exists() && file.isReadable())
                {
                    DesktopEntry d(file.absoluteFilePath());

                    // if a file with this name was hiodden in a more prioritary directory, we have to ignore it
                    if(forbidden_files.contains(file.fileName()))
                        continue;

                    // check we can display it
                    if(!d.onlyShowIn().isEmpty() && !d.onlyShowIn().contains("X-NEBULAE")) {
                        qDebug() << " * " << file.fileName() << " cannot be launched by Nebulae because of its OnlyShowIn key";
                        continue;
                    }
                    if(!d.notShowIn().isEmpty() && d.notShowIn().contains("X-NEBULAE")){
                        qDebug() << " * " << file.fileName() << " cannot be launched by Nebulae because of its NotShowIn key";
                        continue;
                    }

                    // is it hidden ?
                    if(d.hidden()) {
                        // yes, we can not display any file with its name
                        forbidden_files.append(file.fileName());
                        qDebug() << " * " << file.fileName() << " : Hidden file. Ignored.";
                    }

                    QString tryExec = d.tryExec();
                    QString exec = d.exec();
                    if((tryExec.isEmpty() || validBinaryPath(tryExec.split(' ').first())) && validBinaryPath(exec.split(' ').first())) {
                       QProcess p;
                       p.setWorkingDirectory(d.path().isEmpty() ? a.applicationDirPath() : d.path());
                       bool success = simul ? true : p.startDetached(exec.remove("%u", Qt::CaseInsensitive));
                       if(success)
                           qDebug() << "Starting " << file.fileName() << "... [ok]";
                       else
                           qDebug() << "Starting " << file.fileName() << "... [failed]";
                    }
                    else
                        qDebug() << "Starting " << file.fileName() << "... [failed] (bad tryExec or exec key)";
                }
            }
        }
    }

    return a.exec();
}
