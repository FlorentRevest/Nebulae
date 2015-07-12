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

#ifndef NEBULAERECENTFILES_H
#define NEBULAERECENTFILES_H

/**
  * \file nebulae-recentfiles.h
  *
  * This file provides a Freedesktop-compliant way to use the "Recently used" files of the user.
  * The files are centralized so that any application can store the files the user opens, and access the files previously stored.
  * The entries are described in the file $HOME/.recently-used, as defined in the Recent Files specification : http://standards.freedesktop.org/recent-file-spec/recent-file-spec-latest.html
  */

#include <QtCore/QUrl>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>

/**
  * \class RecentFilesItem
  *
  * Describes a single entry of the Recently Used Files.
  * It stores informations like its path, the timestamp of the epoch when he was inserted to the list, and its mimetype.
  */
class RecentFilesItem
{
public:
    /**
      * \brief Initialize an empty entry
      */
    explicit RecentFilesItem() {}

    /**
      * \struct BookmarkApplication
      * \brief Describes an application that registered an entry.
      * \variable name The name of the application
      * \variable exec The command line to launch this application
      * \variable count The number of times this application registered the file
      */
    struct BookmarkApplication {
        QString name;
        QString exec;
        int count;
    };

    // accessors for the properties (see below) //
    /**
      * \brief changes the path of the file described by this entry.
      * \param path The absolute file path to use.
      * \warning Using this property won't write anything to the Recent Files file.
      */
    void setPath(const QString &path);

    /**
      * \brief changes the date when the entry was added to the list.
      * \param time The date time, when the file was inserted to the list.
      * \warning Using this property won't write anything to the Recent Files file.
      */
    void setAdded(const QDateTime &dateTime);

    /**
      * \brief changes the date when the entry was modified.
      * \param time The date time of the last modification.
      * \warning Using this property won't write anything to the Recent Files file.
      */
    void setModified(const QDateTime &dateTime);

    /**
      * \brief changes the date when the entry was visited.
      * \param time The date time of the last time an application opened this file.
      * \warning Using this property won't write anything to the Recent Files file.
      */
    void setVisited(const QDateTime &dateTime);

    /**
      * \brief changes the MimeType of the file described by this entry.
      * \param mimeType The MIME type of the file, as described by Freedesktop here : http://standards.freedesktop.org/shared-mime-info-spec/shared-mime-info-spec-latest.html
      * \warning Using this property won't write anything to the Recent Files file.
      */
    void setMimeType(const QString &mimeType);

    /**
      * \brief add a group to the list of the groups that entry belongs to.
      * \param group The name of the groupŝ.
      * \warning Using this property won't write anything to the Recent Files file.
      */
    void addGroup(const QString &group);

    /**
      * \brief adds an application to the list of the applications that registered the bookmark
      * \param name The name of the application
      * \param exec The command line to launch the application
      * \param count The number of times this applications added the file. If the application already exists, this number will just be increased by one.
      * \warning Using this property won't write anything to the Recent Files file.
      */
    void addApplication(const QString &name, const QString &exec, int count);

    /**
      * \brief Returns the path of the file described by the entry
      */
    QString path();

    /**
      * \brief the date when the entry was added to the list.
      */
    QDateTime added();

    /**
      * \brief the date when the entry was modified.
      */
    QDateTime modified();

    /**
      * \brief the date when the entry was visited.
      */
    QDateTime visited();

    /**
      * \brief Returns the MIME type of the file described by the entry, as described by Freedesktop here : http://standards.freedesktop.org/shared-mime-info-spec/shared-mime-info-spec-latest.html
      */
    QString mimeType();

    /**
      * \brief Returns the lsit of all the groups the entry belongs to
      */
    QStringList groups();

    /**
      * \brief The list of all the applications that registered this file.
      */
    QList<BookmarkApplication> applications();

private:
    // free-desktop defined properties for RecentFiles

    // absolute URI of the file
    QString m_path;

    // time when it was : inserted into the list; modified; visited
    QDateTime m_added, m_modified, m_visited;

    // the mimetype of the file
    QString m_mime_type;

    // the groups that this item belongs to
    QStringList m_groups;

    // the applications that registered this file
    QList<BookmarkApplication> m_apps;
};

/**
  * \class RecentFiles
  *
  * Provides access to the list of all the entries in the Recent Files file, in the shape of a QList of RecentFilesItems
  * Also allows to store new recent files, or modify existing files, by using the addFile() method
  */
class RecentFiles : public QObject
{
    Q_OBJECT

public:
    /**
      * \brief Creates a representation in memory of the $HOME/.recently-used file. Parses the file and allows to access the list of entries
      */
    explicit RecentFiles(QObject *parent = 0);

    // accessors to the items //
    /* be aware: the methods that doesn't specify a group
       won't return a file marked as private
       The list are classed by timestamp (older first) */

    /**
      * \brief Returns all the items
      * \note The list is sorted by timestamp (olders first)
      */
    QList<RecentFilesItem> allItems();

    /**
      * \brief Returns all the items of a given MIME type
      * \param mimeType The MIME type of the file, as described by Freedesktop here : http://standards.freedesktop.org/shared-mime-info-spec/shared-mime-info-spec-latest.html
      * \note The list is sorted by timestamp (olders first)
      */
    QList<RecentFilesItem> allItems(const QString &mimeType);

    /**
      * \brief Returns all the items in the given group.
      * \note The list is sorted by timestamp (olders first)
      */
    QList<RecentFilesItem> items(const QString &group);

    /**
      * \brief Returns all the items in the given group of the specified MIME type.up.
      * \param mimeType The MIME type of the file, as described by Freedesktop here : http://standards.freedesktop.org/shared-mime-info-spec/shared-mime-info-spec-latest.html
      * \note The list is sorted by timestamp (olders first)
      */
    QList<RecentFilesItem> items(const QString &group, const QString &mimeType);

    /**
      \brief Returns the number of items, including privates, IE the number of entries in the XML file
      */
    int count();

    static void addItem(const QUrl &filePath, const QString &app = 0);

signals:
    // emitted when the list is reloaded, and wsend as parameter all files, without private //
    void itemListChanged(QList<RecentFilesItem>);

private slots:
    // used internally to know when the file is modified //
    // void m_file_changed();

private:
    // all the item stored in the file and cached for use when the file is closed
    QList<RecentFilesItem> m_items;

    void m_reload_list();
};

#endif // NEBULAERECENTFILES_H
