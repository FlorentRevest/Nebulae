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

#include "nebulae-recentfiles.h"

#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QCoreApplication>

#include <QtXml/QDomDocument>

#include <QtDebug>

// converts a string containing a UTC time from the format YYYY-MM-DDThh:mm:ssZ to a local QDateTime
QDateTime time(const QString &time)
{
    QDateTime ret;
    ret = QDateTime::fromString(time, "yyyy-MM-ddThh:mm:ssZ");
    ret.setTimeSpec(Qt::UTC);
    return ret.toLocalTime();
}

QList<RecentFilesItem> timeSorted(QList<RecentFilesItem> list)
{
    for(int i = 0; i<list.length(); i++)
    {
        for(int j=i; j < list.length(); j++)
        {
            if(list[j].added() > list[i].added())
            {
                RecentFilesItem it = list[j];
                list[j] = list[i];
                list[i] = it;
            }
        }
    }

    QList<RecentFilesItem> ret;
    foreach(RecentFilesItem it, list)
    {
        qDebug() << it.path();
        if(QFile(it.path()).exists()) {
            qDebug() << it.path() << " exists";
            ret.append(it);
        }
    }

    return ret;
}

// accessors for RecentFilesItem //

void RecentFilesItem::setPath(const QString &path)
{
    m_path = path;
}

void RecentFilesItem::setMimeType(const QString &mimeType)
{
    m_mime_type = mimeType;
}

void RecentFilesItem::setAdded(const QDateTime &dateTime)
{
    m_added = dateTime;
}

void RecentFilesItem::setModified(const QDateTime &dateTime)
{
    m_modified = dateTime;
}

void RecentFilesItem::setVisited(const QDateTime &dateTime)
{
    m_visited = dateTime;
}

void RecentFilesItem::addGroup(const QString &group)
{
    m_groups.append(group);
}

void RecentFilesItem::addApplication(const QString &name, const QString &exec, int count)
{
    BookmarkApplication app;
    app.name = name;
    app.exec = exec;
    app.count = count;
    m_apps.append(app);
}

QString RecentFilesItem::path()
{
    return m_path;
}

QString RecentFilesItem::mimeType()
{
    return m_mime_type;
}

QDateTime RecentFilesItem::added()
{
    return m_added;
}

QDateTime RecentFilesItem::modified()
{
    return m_modified;
}

QDateTime RecentFilesItem::visited()
{
    return m_visited;
}

QStringList RecentFilesItem::groups()
{
    return m_groups;
}

QList<RecentFilesItem::BookmarkApplication> RecentFilesItem::applications()
{
    return m_apps;
}

// main class //

QList<RecentFilesItem> RecentFiles::allItems()
{
    return timeSorted(m_items);
}

QList<RecentFilesItem> RecentFiles::allItems(const QString &mimeType)
{
    QList<RecentFilesItem> ret;
    foreach(RecentFilesItem it, m_items)
    {
        if(it.mimeType() == mimeType)
            ret.append(it);
    }
    return timeSorted(ret);
}

QList<RecentFilesItem> RecentFiles::items(const QString &group)
{
    QList<RecentFilesItem> ret;
    foreach(RecentFilesItem it, m_items)
    {
        if(it.groups().contains(group))
            ret.append(it);
    }
    return timeSorted(ret);
}

QList<RecentFilesItem> RecentFiles::items(const QString &group, const QString &mimeType)
{
    QList<RecentFilesItem> ret;
    foreach(RecentFilesItem it, m_items)
    {
        if(it.groups().contains(group) && it.mimeType() == mimeType)
            ret.append(it);
    }
    return timeSorted(ret);
}

void RecentFiles::m_reload_list()
{
    m_items.clear();

    // opens the file containing the bookmarks
    // TODO: add management of other files, stored at a system level
    QFile fi(QDir::homePath() + "/.recently-used.xbel");
    if(!fi.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    // the file was sucessfully opened
    QDomDocument *doc = new QDomDocument();
    doc->setContent(&fi);
    QDomElement el = doc->documentElement();
    if(!(el.tagName() == "xbel") && !(el.attribute("version") == "1.0"))
        return;

    el = el.firstChildElement();
    while(!el.isNull() && el.tagName() == "bookmark")
    {
        RecentFilesItem f;
        f.setPath(el.attribute("href"));

        f.setAdded(time(el.attribute("added")));
        f.setModified(time(el.attribute("modified")));
        f.setVisited(time(el.attribute("visited")));

        // read metadatas
        QDomElement meta = el.firstChildElement().firstChildElement();
        if(meta.tagName() == "metadata" && meta.attribute("owner") == "http://freedesktop.org")
        {
            meta = meta.firstChildElement();
            while(!meta.isNull())
            {
                if(meta.tagName() == "mime:mime-type")
                    f.setMimeType(meta.attribute("type"));
                else if(meta.tagName() == "bookmark:groups")
                {
                    QDomElement group_el = meta.firstChildElement();
                    while(!group_el.isNull() && group_el.tagName() == "bookmark:group")
                    {
                        f.addGroup(group_el.text());
                        group_el = group_el.nextSiblingElement();
                    }
                }
                else if(meta.tagName() == "bookmark:applications")
                {
                    QDomElement app_el = meta.firstChildElement();
                    while(!app_el.isNull() && app_el.tagName() == "bookmark:application")
                    {
                        f.addApplication(app_el.attribute("name"), app_el.attribute("exec"), app_el.attribute("count").toInt());
                        app_el = app_el.nextSiblingElement();
                    }
                }
                meta = meta.nextSiblingElement();
            }
        }
        m_items.append(f);
        el = el.nextSiblingElement();
    }

    fi.close();
}

void RecentFiles::addItem(const QUrl &file, const QString &app)
{
    QString app_name = app.isEmpty() ? qApp->applicationName() : app;

            // we have to update it in the file
            // opens the file containing the bookmarks
            // TODO: add management of other files, stored at a system level
            QFile fi(QDir::homePath() + "/.recently-used.xbel");
            if(!fi.open(QIODevice::ReadOnly | QIODevice::Text))
                return;

            // the file was sucessfully opened
            QDomDocument *doc = new QDomDocument();
            doc->setContent(&fi);
            QDomElement el = doc->documentElement();
            if(!(el.tagName() == "xbel") && !(el.attribute("version") == "1.0"))
                return;

            el = el.firstChildElement();
            while(!el.isNull() && el.tagName() == "bookmark")
            {
                if(el.attribute("href") == file.toString()) // we reached the entry
                {
                    // update the last time
                    el.setAttribute("modified", QDateTime::currentDateTimeUtc().toString("yyyy-MM-ddThh:mm:ssZ"));

                    // read metadatas
                    QDomElement meta = el.firstChildElement().firstChildElement();
                    if(meta.tagName() == "metadata" && meta.attribute("owner") == "http://freedesktop.org")
                    {
                        meta = meta.firstChildElement();
                        while(!meta.isNull())
                        {
                            if(meta.tagName() == "bookmark:applications")
                            {
                                QDomElement app_el = meta.firstChildElement();
                                bool already_in_file = false;
                                while(!app_el.isNull() && app_el.tagName() == "bookmark:application")
                                {
                                    if(app_el.attribute("name") == app_name) {
                                        already_in_file = true;
                                        app_el.setAttribute("count", app_el.attribute("count").toInt()+1);
                                    }
                                    app_el = app_el.nextSiblingElement();
                                }
                                if(!already_in_file) { // add it
                                    QDomElement new_el;
                                    new_el.setAttribute("name", app_name);
                                    new_el.setAttribute("exec", app_name + " %u");
                                    new_el.setAttribute("count", 1);
                                }
                            }
                            meta = meta.nextSiblingElement();
                        }
                    }
                }
            }

}

RecentFiles::RecentFiles(QObject *parent) : QObject(parent)
{
    // loads the items from the file
    m_reload_list();
}
