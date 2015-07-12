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

#include "nebulae-mimetypeinfo.h"
#include "nebulae-desktopentry.h"

#include <QtCore/QFile>
#include <QtCore/QSettings>

#include <QtGui/QIcon>

#include <QtDebug>

// returns the desktop entry for the firts element of this list.
// if not found, try with the second one, etc.
QList<DesktopEntry> desktopEntries(QStringList names)
{
    QList<DesktopEntry> ret;

    foreach(QString name, names)
    {
        if(QFile(XDG_DATA_HOME + "/applications/" + name).exists())
            ret.append(DesktopEntry(XDG_DATA_HOME + "/applications/" + name));

        foreach(QString dir, XDG_DATA_DIRS)
        {
            if(QFile(dir + "/applications/" + name).exists())
                ret.append(DesktopEntry(dir + "/applications/" + name));
        }
    }

    return ret;
}

// internal convenience function
// returns all the dirs in which MIME type informations are stored
QStringList mimeDirs()
{
    QStringList ret;

    // according to Freedesktop, those dirs are located in XDG_DATA_HOME/mime:XDG_DATA_DIRS/mime
    foreach(QString dir, XDG_DATA_DIRS)
        ret.append(dir + "/mime");
    ret.append(XDG_DATA_HOME + "/mime");

    return ret;
}

// MimeType class accessors implementation //

QString MimeType::name()
{
    return m_name;
}

QString MimeType::subclass()
{
    return m_subclass;
}

QString MimeType::icon()
{
    // is alias ? yes -> icon of the target / no -> given icon
    QString ret = m_alias ? nApp->mimeTypeDatabase()->findType(m_alias_target).icon() : m_icon;

    // nothing found ? use the subclasses's one (if any)
    if(ret.isEmpty() && !m_subclass.isEmpty())
        ret = nApp->mimeTypeDatabase()->findType(m_subclass).icon();

    return ret;
}

QStringList MimeType::extensions()
{
    // as above, with the ext list
    QStringList ret = m_alias ? nApp->mimeTypeDatabase()->findType(m_alias_target).extensions() : m_ext;

    if(ret.isEmpty() && !m_subclass.isEmpty())
        ret = nApp->mimeTypeDatabase()->findType(m_subclass).extensions();

    return ret;
}

bool MimeType::isAlias()
{
    return m_alias;
}

QString MimeType::aliasTarget()
{
    return m_alias ? m_alias_target : QString();
}

// Mime Type Database methods implementation //

MimeType MimeTypeDatabase::findType(const QString &name)
{
    // find a type if the cache
    foreach(MimeType type, m_cache)
    {
        if(type.name() == name)
            return type;
    }

    // if not found, return a blank type to avoid segfaults
    return MimeType("","","",QStringList(),"");
}

MimeType MimeTypeDatabase::fileType(QFileInfo file)
{
    // determine the type by the file extension
    // TODO: add the possibility to determine in by the magic number
    QString suffix = "*." + file.completeSuffix();

    QList<MimeType> ret;

    // if the file is an hidden file (IE begins with '.'), QFileInfo will
    // consider the whole file name as a suffix. This fixes it :
    if(file.fileName().startsWith('.'))
    {
        QStringList prts = file.completeSuffix().split('.');
        prts.removeFirst();
        prts.removeFirst();
        suffix = prts.join(".");
    }

    // run a first try with the given name, as it
    foreach(MimeType type, m_cache)
    {
        if(type.extensions().contains(suffix))
            ret.append(type);
    }

    // retry with the same extension converted into lowercaser
    if(ret.isEmpty())
    {
        suffix = suffix.toLower();
        foreach(MimeType type, m_cache)
        {
            if(type.extensions().contains(suffix))
                ret.append(type);
        }
    }

    if(ret.count() == 1)
        return ret.first();

    // Freedesktop imposes not to rely on of the types if several are found
    // but to user the magic number istead.
    // TODO: implement it!
    return ret.first();
}

QList<DesktopEntry> MimeTypeDatabase::programsForType(MimeType _type)
{
    QString type = _type.name();

    QStringList forbidden_apps;
    QStringList apps;
    QSettings s(XDG_DATA_HOME + "/applications/mimeapps.list", QSettings::NativeFormat);

    s.beginGroup("Removed Associations");
    foreach(QString app, s.value(type).toString().split(';'))
    {
        if(!app.isEmpty())
            forbidden_apps.append(apps);
    }
    s.endGroup();
    s.beginGroup("Added Associations");
    foreach(QString app, s.value(type).toString().split(';'))
    {
        if(!app.isEmpty() && !forbidden_apps.contains(app))
            apps.append(app);
    }
    QSettings sys("/usr/share/applications/mimeapps.list", QSettings::NativeFormat);
    s.beginGroup("Removed Associations");
    foreach(QString app, sys.value(type).toString().split(';'))
    {
        if(!app.isEmpty())
            forbidden_apps.append(apps);
    }
    s.endGroup();
    s.beginGroup("Added Associations");
    foreach(QString app, sys.value(type).toString().split(';'))
    {
        if(!app.isEmpty() && !forbidden_apps.contains(app))
            apps.append(app);
    }

    QSettings def("/usr/share/applications/defaults.list", QSettings::NativeFormat);
    def.beginGroup("Default Applications");
    QString app(def.value(type).toString());
    if(!app.isEmpty() && !forbidden_apps.contains(type))
        apps.append(app);

    return desktopEntries(apps);
}

QString MimeTypeDatabase::description(MimeType type, QString locale)
{
    QString name = type.name();

    QString ret;
    QString default_ret;

    foreach(QString dir, mimeDirs())
    {
        QFile f(dir + "/" + name + ".xml");
        if(QFileInfo(f.fileName()).isFile() && f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            while(!f.atEnd())
            {
                QString line = QString::fromUtf8(f.readLine());

                // remove the ending \n
                if(line.endsWith('\n'))
                    line.truncate(line.length()-1);

                if(!line.contains("<comment"))
                    continue;       // performances fix : don't even try lines that does not contains this

                // remove the trailing spaces
                while(line.startsWith(' '))
                    line = line.mid(1);

                // now, is the syntax correct
                if(line.startsWith("<comment>") && line.endsWith("</comment>"))
                    default_ret = line.split('>').at(1).split('<').first(); // comment in the default langage
                else if(line.startsWith("<comment xml:lang=\"" + locale + "\">") && line.endsWith("</comment>"))
                    ret = line.split('>').at(1).split('<').first();
                }
        }
    }
    // if not found in the given locale, return the default one;
    return ret.isEmpty() ? default_ret : ret;
}

DesktopEntry MimeTypeDatabase::programForType(MimeType type)
{
    QList<DesktopEntry> ret = programsForType(type);
    return ret.isEmpty() ? DesktopEntry("") : ret.first();
}

QList<MimeType> MimeTypeDatabase::m_update_cache()
{
    // update the cache from the files
    // prepares hashes to get every informations we can
    // we will then mix them into a new cache
    QList<MimeType> cache;
    QHash<QString, bool> known_types;       // bool = alias
    QHash<QString,QString> known_icons;
    QHash<QString,QString> known_classes;
    QHash<QString,QString> aliases_targets;
    QHash<QString,QStringList> known_exts;

    // TODO: find a better way to detect incorrent lines in files than startsWith('#'). A RegExp ?

    // get a full list of all known types (without aliases)
    foreach(QString dir, mimeDirs())
    {
        QFile f(dir + "/types");
        if(f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            while(!f.atEnd())
            {
                QString line = QString::fromUtf8(f.readLine());

                if(line.startsWith('#'))
                    continue;

                if(line.endsWith('\n'))
                    line.truncate(line.length()-1);

                known_types.insert(line, false); // this file does not contains any aliases
            }
        }
        f.close();
    }

    // now, which types have subclasses
    foreach(QString dir, mimeDirs())
    {
        QFile f(dir + "/subclasses");
        if(f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            while(!f.atEnd())
            {
                QString line = QString::fromUtf8(f.readLine());

                if(line.startsWith('#'))
                    continue;

                if(line.endsWith('\n'))
                    line.truncate(line.length()-1);

                QString type = line.split(' ').first();
                QString subclass = line.split(' ').at(1);

                known_classes.insert(type, subclass);
            }
        }
    }

    // get more types (the aliases)
    foreach(QString dir, mimeDirs())
    {
        QFile f(dir + "/aliases");
        if(f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            while(!f.atEnd())
            {
                QString line = QString::fromUtf8(f.readLine());

                if(line.startsWith('#'))
                    continue;

                if(line.endsWith('\n'))
                    line.truncate(line.length()-1);

                QString alias_name = line.split(' ').first();
                QString target = line.split(' ').at(1);

                aliases_targets.insert(alias_name, target);
                known_types.insert(alias_name, true); // this file contains only aliases
            }
        }
        f.close();
    }

    // get the extensions that corresponds to any type
    foreach(QString dir, mimeDirs())
    {
        QFile f(dir + "/globs2");
        if(f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            while(!f.atEnd())
            {
                QString line = QString::fromUtf8(f.readLine());

                if(line.startsWith('#'))
                    continue;

                if(line.endsWith('\n'))
                    line.truncate(line.length()-1);

                QString type = line.split(':').at(1);

                QStringList exts = known_exts.keys().contains(type) ? known_exts.value(type) : QStringList();
                exts << line.split(':').at(2);

                known_exts.remove(type);
                known_exts.insert(type, exts);
            }
        }
        f.close();
    }

    // the icons
    foreach(QString dir, mimeDirs())
    {
        QFile f(dir + "/generic-icons");
        if(f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            while(!f.atEnd())
            {
                QString line = QString::fromUtf8(f.readLine());

                if(line.startsWith('#'))
                    continue;

                if(line.endsWith('\n'))
                    line.truncate(line.length()-1);

                QString type = line.split(':').first();
                QString icon = line.split(':').at(1);
                known_icons.insert(type, icon);
            }
        }
        f.close();
    }

    // mix all previously stored data
    foreach(QString type, known_types.keys())
        cache.append(MimeType(type, known_icons.value(type), known_classes.value(type), known_exts.value(type), aliases_targets.value(type)));

    return cache;
}

MimeTypeDatabase::MimeTypeDatabase()
{
    // just create a new cache
    m_cache.append(m_update_cache());
}
