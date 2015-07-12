#ifndef DESKTOPENTRY_H
#define DESKTOPENTRY_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

#include <QtGui/QIcon>

// this class describes a .desktop freedesktop entry that corresponds to an application //
// it stores its localized name and

class DesktopEntry
{

public:
    explicit DesktopEntry(const QString &filepath);

    QString name()
    {
        return m_name;
    }

    QString genericName()
    {
        return m_generic_name;
    }

    QString description()
    {
        return m_description;
    }

    QIcon icon()
    {
        return m_icon;
    }

    QString filename()
    {
        return m_filename;
    }

    QStringList mimetypes()
    {
        return m_mimetypes;
    }

private:
    // some freedesktop properties //

    QString m_name, m_generic_name, m_description, m_filename;
    QStringList m_mimetypes;
    QIcon m_icon;

};

#endif // DESKTOPENTRY_H
