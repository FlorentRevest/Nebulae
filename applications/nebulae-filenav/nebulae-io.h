#ifndef NEBULAEIO_H
#define NEBULAEIO_H

#include <QtCore/QUrl>
#include <QtCore/QObject>
#undef signals
#include <gio/gio.h>

class FileInfo
{

public:
    explicit FileInfo() {}
    explicit FileInfo(const QString &uri);
    explicit FileInfo(GFileInfo*);
    ~FileInfo();

    QString fileName();
    QString displayName();

    QString baseName();
    QString suffix();
    QString completeSuffix();

    QUrl    absoluteUri();
    QString absoluteFilePath();
    QString absolutePath();

    QString iconName();

    bool name();
    bool isFile();
    bool isDir();
    bool isSymlink();

    bool isReadable();
    bool isWritable();
    bool isExecutable();

    void setGFileInfo(GFileInfo *);

private:
    QUrl m_uri;
    GFileInfo *m_info;
    GVfs *m_vfs;
};
typedef QList<FileInfo> FileInfoList;

class File : public QObject
{
    Q_OBJECT
public:
    explicit File(const QString &uri);
    ~File();

    QString uri();

private:
    QString m_uri;
    GFile *m_file;
    FileInfo *m_info;
    GVfs *m_vfs;
};

/**
  * \class Dir
  * \brief Provides access to directory structures and their contents
  *
  * This class is a re-implementation of the Qt class QDir using the
  * GVFS library to access virtual file systems such as network file
  * systems (SMB, SFTP...), the Freedesktop trash (trash://), etc.
  */
class Dir
{
public:

    /**
      * \enum Filter
      * This enum describes the filtering options available to Dir; e.g.
      * for entryList() and entryInfoList(). The filter value is specified
      * by combining values from the following list using the bitwise OR operator:
      */
    enum Filters {
        Dirs,
        AllDirs,
        Files,
        AllFiles,
        AllEntries,
        NoSymlinks,
        Readable,
        Writeable,
        Executable,
        Hidden,
        NoFilter
    };
    enum SortFlags {
        NoSort
    };

    /**
      * \brief Builds a new Dir with the given URI.
      * \param uri the full uri of a directory of the shape protocol://user:password@host:port/path
      */
    explicit Dir(const QString &uri);
    ~Dir();

    /**
      * \brief Accessor the the URI of the directory.
      * \return a uri of the shape protocol://user:password@host:port/path
      */
    QString uri();

    /**
      * \brief Lists the contents of the directory.
      * \return a QStringList containing the basename of all entries
      */
    QStringList entryList(QFlags<Filters> filters = NoFilter, QFlags<SortFlags> sortFlags = NoSort);
    FileInfoList entryInfoList(QFlags<Filters> filters = NoFilter, QFlags<SortFlags> sortFlags = NoSort);

    void setFilters(QFlags<Filters>);
    QFlags<Filters> filters();

    void setSortFlags (QFlags<SortFlags>);
    QFlags<SortFlags> sortFlags();

    bool exists();

private:
    // interal properties //
    QString m_uri;
    GFile *m_file;
    FileInfo *m_info;
    GVfs *m_vfs;

    // default filters and sort flags used by entryList //
    // can be overriden with filters and sortFlags args //
    QFlags<Filters> m_filters;
    QFlags<SortFlags> m_sort_flags;

    // used by entryList(). Return a list of entries without //
    // any filter or flags to be filtered and sorted after...//
    QStringList m_entry_list();
};

#endif // NEBULAEIO_H
