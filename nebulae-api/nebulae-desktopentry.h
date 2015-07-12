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

#ifndef NEBULAEDESKTOPENTRY_H
#define NEBULAEDESKTOPENTRY_H

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

#include <QtGui/QIcon>

class QFileSystemWatcher;

/**
  * \class DesktopEntry
  *
  * This class describes a Freedesktop Desktop Entry \link http://standards.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html
  *
  * A DesktopEntry is a file (usually .desktop) that is used to launch applications or open directories and URLs.
  * Those files include informations such as the name (eg: Firefox), the generic name (eg: Web Browser), and a comment (eg: Browse the Internet) translated into many locales.
  * It also contains informations about how to run the DesktopEntry, with the path to a program to exec, an URL, which environnements should display this entry, etc.
  * This class is a representation in memory of this entry, that allows to access the various freedesktop-defined properties by using accessors.
  * It is possible to monitor the file for changes, but optionnal. It should not be user for any files since it uses a lot of CPU.
  * Actually, this class does not allow to physically change the files, as most of them are located in non-writable directories.
  */
class DesktopEntry
{

public:
    /**
      * \brief Creates a representation of the entry from the given filePath.
      * \param filePath The absolute file path of the filesystem of the file that informations will be read from.
      */
    explicit DesktopEntry(const QString &filePath);

    /**
      * \enum Type
      * \brief Possible values for the "Type" property
      */
    enum Type {
        Application,    /*!< Application This DesktopEntry is a launcher for an application, or a program */
        Link,           /*!< Link This DesktopEntry is a link to a given URL. */
        Directory,      /*!< Directory This DesktopEntry opens a directory. */
        Unknown         /*!< To allow the addition of new types in the future, implementations should ignore desktop entries with an unknown type.  */
    };

    /**
      * \brief The Type property of the desktop entry. See DesktopEntry::Type
      * \return Application if the entry is a launcher for an application, Link if it's a link to a URL, or Directory for a shortcut to a local folder.
      */
    Type type();

    /**
      * \brief Returns the Name property of the desktop entry
      * \return The name of the application, in the default locale, such as "Web Browser Firefox", or "Nebulae Admin Panel"
      */
    QString name();

    /**
      * \brief Returns the localized name property of the desktop entry, like Name[fr] for french-translated name
      * \param locale The name of the locale (like fr or en_US).
      * \return The name of the application, in the given locale */
    QString name(const QString &locale);

    /**
      * \brief Returns the GenericName property of the desktop entry
      * \returns The generic name of the application, I.E. a name that could be used for any application of the same type, like "Web browser" or "Text editor"
      */
    QString genericName();

    /**
      * \brief Returns the localized generic name property of the desktop entry, like GenericName[fr] for the french-translated generic name
      * \param locale The name of the locale (like fr or en_US).
      * \return The generic name of the application, in the given locale
      */
    QString genericName(const QString &locale);

    /**
      * \brief Returns the Comment property of the desktop entry
      * \return A small comment taht describes what this application does, for example "Browse the Internet" or "Edit simple text files"
      */
    QString comment();

    /**
      * \brief Returns the localized comment property of the desktop entry, like Comment[fr] for french-translated comment.
      * \param locale The name of the locale (like fr or en_US).
      * \return A small comment taht describes what this application does, in the given locale
      */
    QString comment(const QString &locale);

    /**
      * \brief Returns the tryExec property of the desktop entry.
      * \return The absolute file path (or the name, if the file is located in $PATH), of a binary that sould determine if the application is present.
      */
    QString tryExec();

    /**
      * \brief Returns the exec property of the desktop entry.
      * \return The absolute file path (or the name, if the file is located in $PATH), of the main binary of the application. Only applies when Type is Application.
      */
    QString exec();

    /**
      * \brief Returns the Hidden property of the entry
      * \return true if this entry is hidden, IE it is present but should be considered as deleted. false otherwise.
      */
    bool hidden();

    /**
      * \brief Returns the OnlyShowIn property of the entry.
      * \return a list of environnement that _must_ display this entry. All other environnements should not display it at all. Environnements are define here : http://standards.freedesktop.org/menu-spec/menu-spec-latest.html#onlyshowin-registry
      */
    QStringList onlyShowIn();

    /** \brief Returns the NotShowIn property of the entry.
      * \return a list of environnement that _must not_ display this entry. All other environnements _must_ display it. Environnements are define here : http://standards.freedesktop.org/menu-spec/menu-spec-latest.html#onlyshowin-registry
      */
    QStringList notShowIn();

    /**
      * \brief The Path property of the entry.
      * \return The absolute file path of a directory that should be sued as CWD when launching the program specified by the exec property. Only applies when Type is Application.
      */
    QString path();

    /**
      * \brief The Icon of the entry
      * \return a QIcon containing the icon taht should be displayed to represent the entry in a menu or a list.
      */
    QIcon icon();

    /**
      * \brief The Terminal property
      * \return true if the entry must be launched in a terminal, otherwise returns false.
      */
    bool terminal();

    /**
      * \brief The MimeType property
      * \return a list of all the MIME types this application can be used to open. For more details about MIME types, see : http://standards.freedesktop.org/shared-mime-info-spec/shared-mime-info-spec-latest.html
      */
    QStringList mimeTypes();

    /**
      * \brief The Categories property
      * \return a list of the name of all the categories this entry belongs to. A list of freedesktop-recognized categories is available here : http://standards.freedesktop.org/menu-spec/latest/apa.html
      */
    QStringList categories();

    /**
      * \brief The StartupNotify property
      * \return true if the start up of this application should be notified to the user, otherwise returns false. Fore more details, see http://www.freedesktop.org/wiki/Specifications/startup-notification-spec?action=show&redirect=Standards%2Fstartup-notification-spec
      */
    bool startupNotify();

    /**
      * \brief The StartupWMClass property
      * \returns the WM class, see http://www.freedesktop.org/wiki/Specifications/startup-notification-spec?action=show&redirect=Standards%2Fstartup-notification-spec fore more details.
      */
    QString startupWMClass();

    /**
      * The Url property.
      * \return The URL the entry points to, if Type is Link.
      */
    QUrl url();

    /**
      * \brief The file path of the entry.
      * \return the absolute file path of the file the entry was loaded from.
      */
    QString filePath();

//private slots:
    // called when the FileSystemWatcher detects a modification on the file //
    // void m_file_modified();

private:
    void m_read_informations_from_file();

    // internal : a QString that represents the address to the file //
    QString m_desktop_file_path;

    // and an object to watchchanges on this file //
    QFileSystemWatcher *m_watcher;

    // FreeDesktop defined properties //
    Type                          m_type;
    QString                       m_name;
    QHash<QString,QString>        m_localized_name;
    QString                       m_generic_name;
    QHash<QString,QString>        m_localized_generic_name;
    bool                          m_no_display;
    QString                       m_comment;
    QHash<QString,QString>        m_localized_comment;
    QIcon                         m_icon;
    bool                          m_hidden;
    QStringList                   m_only_show_in;
    QStringList                   m_not_show_in;
    QString                       m_try_exec;
    QString                       m_exec;
    QString                       m_path;
    bool                          m_terminal;
    QStringList                   m_mime_types;
    QStringList                   m_categories;
    bool                          m_startup_notify;
    QString                       m_startup_wm_class;
    QString                       m_url;

    // non Freedesktop properties //
    QHash<QString,QString>       m_non_fdesktop_props;
};

#endif // NEBULAEDESKTOPENTRY_H
