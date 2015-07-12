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

#include "nebulae-desktopentry.h"
#include "nebulae-freedesktopsettings.h"

#include <QtCore/QFile>

#include <QtDebug>

DesktopEntry::Type gettype(const QString &type)
{
    if(type == "Application")
        return DesktopEntry::Application;
    else if(type == "Link")
        return DesktopEntry::Link;
    else if(type == "Directory")
        return DesktopEntry::Directory;
    else
        return DesktopEntry::Unknown;
}

QIcon geticon(const QString &icon)
{
    QIcon ic;
    if(QFile::exists(icon))
        ic = QIcon(icon);
    if(ic.isNull())
        ic = QIcon::fromTheme(icon);
    return ic;
}

void DesktopEntry::m_read_informations_from_file()
{
    // insert default values
    m_terminal = false;
    m_hidden = false;
    m_no_display = false;
    m_terminal = false;

    FreedesktopSettings set(m_desktop_file_path);
    set.beginGroup("Desktop Entry");
    foreach(QString prop, set.allKeys())
    {
        if(prop == "Type")
            m_type = gettype(set.value(prop).toString());
        if(prop == "Name")
            m_name = set.value(prop).toString();
        else if(prop.startsWith("Name[")) {
            QStringList prts = prop.split('[');
            prts.removeFirst();
            QString loc = prts.first().split(']').takeFirst();
            m_localized_name.insert(loc, set.value(prop).toString());
        }
        else if(prop == "GenericName")
            m_generic_name = set.value(prop).toString();
        else if(prop.startsWith("GenericName[")){
            QStringList prts = prop.split('[');
            prts.removeFirst();
            QString loc = prts.first().split(']').takeFirst();
            m_localized_name.insert(loc, set.value(prop).toString());
        }
        else if(prop == "Comment")
            m_comment = set.value(prop).toString();
        else if(prop.startsWith("Comment[")){
            QStringList prts = prop.split('[');
            prts.removeFirst();
            QString loc = prts.first().split(']').takeFirst();
            m_localized_name.insert(loc, set.value(prop).toString());
        }
        else if(prop == "NoDisplay")
            m_no_display = (set.value(prop).toString() == "true") ? true : false;
        else if(prop == "Icon")
            m_icon = geticon(set.value(prop).toString());
        else if(prop == "Hidden")
            m_no_display = (set.value(prop).toString() == "true") ? true : false;
        else if(prop == "OnlyShowIn")
            m_only_show_in.append(set.value(prop).toString().split(';'));
        else if(prop == "NotShowIn")
            m_not_show_in.append(set.value(prop).toString().split(';'));
        else if(prop == "TryExec")
            m_try_exec = set.value(prop).toString();
        else if(prop == "Exec")
            m_exec = set.value(prop).toString();
        else if(prop == "Path")
            m_path = set.value(prop).toString();
        else if(prop == "Terminal")
            m_terminal = (set.value(prop).toString() == "true") ? true : false;
        else if(prop == "MimeType") {
            m_mime_types.append(set.value(prop).toString().split(';'));
            if(m_mime_types.last().isEmpty())
                m_mime_types.removeLast();
        }
        else if(prop == "Categories") {
            m_categories.append(set.value(prop).toString().split(';'));
            if(m_categories.last().isEmpty())
                m_categories.removeLast();
        }
        else if(prop == "StartupNotify")
            m_startup_notify = (set.value(prop).toString() == "true") ? true : false;
        else if(prop == "StartupWMClass")
            m_startup_wm_class = set.value(prop).toString();
        else if(prop == "URL")
            m_url = set.value(prop).toString();
    }
}

DesktopEntry::DesktopEntry(const QString &filePath)
{
    m_desktop_file_path = filePath;

    // just populate the informations from the file
    m_read_informations_from_file();

    //*
    // debug : print everything
    qDebug() << "Name             " << m_name;
    foreach(QString val, m_localized_name.keys())
        qDebug() << val << m_localized_name.value(val);
    qDebug() << "GenericName      " << m_generic_name;
    foreach(QString val, m_localized_generic_name.keys())
    qDebug() << val << m_localized_generic_name.value(val);
    qDebug() << "NoDisplay        " << m_no_display;
    qDebug() << "Comment          " << m_comment;
    foreach(QString val, m_localized_comment.keys())
    qDebug() <<  val << m_localized_comment.value(val);
    qDebug() << "Icon             " << m_icon.name();
    qDebug() << "Hidden           " << m_hidden;
    qDebug() << "OnlyShowIn       " << m_only_show_in;
    qDebug() << "NotShowIn        " << m_not_show_in;
    qDebug() << "TryExec          " << m_try_exec;
    qDebug() << "Exec             " << m_exec;
    qDebug() << "Path             " << m_path;
    qDebug() << "Terminal         " << m_terminal;
    qDebug() << "MimeType         " << m_mime_types;
    qDebug() << "Categories       " << m_categories;
    qDebug() << "StartupNotify    " << m_startup_notify;
    qDebug() << "StartupWMClass   " << m_startup_wm_class;
    qDebug() << "URL              " << m_url;
    //*/
}

// accessors for properties //

DesktopEntry::Type DesktopEntry::type()
{
    return m_type;
}

QString DesktopEntry::name()
{
    return name(QString());
}

QString DesktopEntry::name(const QString &locale)
{
    QString ret = m_localized_name.value(locale);
    if(ret.isEmpty() || locale.isEmpty())
        return m_name;
    else
        return ret;
}

QString DesktopEntry::genericName()
{
    return genericName(QString());
}

QString DesktopEntry::genericName(const QString &locale)
{
    QString ret = m_localized_generic_name.value(locale);
    if(ret.isEmpty() || locale.isEmpty())
        return m_generic_name;
    else
        return ret;
}

QString DesktopEntry::comment()
{
    return comment(QString());
}

QString DesktopEntry::comment(const QString &locale)
{
    QString ret = m_localized_comment.value(locale);
    if(ret.isEmpty() || locale.isEmpty())
        return m_comment;
    else
        return ret;
}

QString DesktopEntry::tryExec()
{
    return m_try_exec;
}

QString DesktopEntry::exec()
{
    return m_exec;
}

bool DesktopEntry::hidden()
{
    return m_hidden;
}

QStringList DesktopEntry::onlyShowIn()
{
    return m_only_show_in;
}

QStringList DesktopEntry::notShowIn()
{
    return m_not_show_in;
}

QString DesktopEntry::path()
{
    return m_path;
}

QIcon DesktopEntry::icon()
{
    return m_icon;
}

bool DesktopEntry::terminal()
{
    return m_terminal;
}

QStringList DesktopEntry::mimeTypes()
{
    return m_mime_types;
}

QStringList DesktopEntry::categories()
{
    return m_categories;
}

bool DesktopEntry::startupNotify()
{
    return m_startup_notify;
}

QString DesktopEntry::startupWMClass()
{
    return m_startup_wm_class;
}

QUrl DesktopEntry::url()
{
    return QUrl(m_url);
}

QString DesktopEntry::filePath()
{
    return m_desktop_file_path;
}

