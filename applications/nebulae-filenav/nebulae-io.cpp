#include "nebulae-io.h"

#include <QtCore/QFlags>
#include <QtCore/QStringList>

#include <QtDebug>

bool gbooleanToBool(gboolean b)
{
    if(b)
        return true;
    else
        return false;
}

File::File(const QString &uri) : m_uri(uri)
{
    m_vfs = g_vfs_get_default();

    m_file = g_vfs_get_file_for_uri(m_vfs, uri.toStdString().c_str());

    m_info = new FileInfo(uri);
}

File::~File()
{
}

QString File::uri()
{
    return m_uri;
}

Dir::Dir(const QString &uri) : m_uri(uri)
{
    m_vfs = g_vfs_get_default();

    m_file = g_vfs_get_file_for_uri(m_vfs, uri.toStdString().c_str());

    m_info = new FileInfo(uri);

    m_filters = AllEntries;
    m_sort_flags = NoSort;
}

Dir::~Dir()
{
}

bool Dir::exists()
{
    return gbooleanToBool(g_file_query_exists(m_file, NULL));
}

QString Dir::uri()
{
    return m_uri;
}

QFlags<Dir::Filters> Dir::filters()
{
    return m_filters;
}

QFlags<Dir::SortFlags> Dir::sortFlags()
{
    return m_sort_flags;
}

void Dir::setFilters(QFlags<Filters> filters)
{
    m_filters = filters;
}

void Dir::setSortFlags(QFlags<SortFlags> sortFlags)
{
    m_sort_flags = sortFlags;
}

QStringList Dir::m_entry_list()
{
    QStringList entries;

    if(m_info->isDir())
    {
        GError *err = NULL;
        GFileEnumerator *enumerator = g_file_enumerate_children(m_file, "*", G_FILE_QUERY_INFO_NONE, NULL, &err);
        if(!enumerator)
        {
            g_object_unref(enumerator);
            return entries;
        }

        for(GFileInfo *info = g_file_enumerator_next_file(enumerator, NULL, &err); !(info == 0); info = g_file_enumerator_next_file(enumerator , NULL, &err))
        {
            entries.append(QString(g_file_get_uri(m_file)) + "/" + QString(g_file_info_get_name(info)));
            g_object_unref(info);
        }
        g_file_enumerator_close(enumerator, NULL, &err);
        g_object_unref(enumerator);
    }

    return entries;
}

QStringList Dir::entryList(QFlags<Filters> filters,
                           QFlags<SortFlags> sortFlags)
{
    if(filters == NoFilter)
        filters = m_filters;
    if(sortFlags == NoSort)
        sortFlags == m_sort_flags;

    QStringList ret;

    foreach(FileInfo i, entryInfoList(filters, sortFlags))
        ret.append(i.fileName());

    return ret;
}

FileInfoList Dir::entryInfoList(QFlags<Filters> filters,
                           QFlags<SortFlags> sortFlags)
{
    if(filters == NoFilter)
        filters = m_filters;
    if(sortFlags == NoSort)
        sortFlags == m_sort_flags;

    FileInfoList ret;

    // use our internal method to get an unsorted list, then sort it
    foreach(QString entry, m_entry_list())
    {
        // this list contains absolute URIs
        FileInfo i(entry);

        // apply the filters //

        // is this a hidden file ?
        if(i.fileName().startsWith('.') && !filters.testFlag(Hidden))
            continue; // yes, don't include it unless Hidden flag is set

        // check permissions
        if(!i.isReadable() && filters.testFlag(Readable))
            continue; // non readable, and we must ony include readable
        // as above, for +w and +x
        if(!i.isWritable() && filters.testFlag(Writeable))
            continue;
        if(!i.isExecutable() && filters.testFlag(Executable))
            continue;

        // is it a symlink ?
        if(i.isSymlink())
        {
            // can we include it ?
            if(!filters.testFlag(NoSymlinks))
                ret.append(i);
        }

        // is it a dir ?
        else if(i.isDir())
        {
            if(filters.testFlag(AllDirs))
                ret.append(i); // yes, but flag AllDirs tells we should include it even he does not match name filters
            else if(filters.testFlag(Dirs) || filters.testFlag(AllEntries)/* && TODO: add nameFilters */)
                ret.append(i); // yes, but he must match the name filters to be included
        }

        // is it a file ?
        else if(i.isFile())
        {
            if(filters.testFlag(Files) || filters.testFlag(AllEntries))
                ret.append(i);
        }
    }

    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

FileInfo::FileInfo(const QString &uri) : m_uri(QUrl(uri)) // keep the uri to determine path
{
    m_vfs = g_vfs_get_default();

    GFile *m_file = g_vfs_get_file_for_uri(m_vfs, uri.toStdString().c_str());

    GError *err;
    m_info = g_file_query_info(m_file, "*", G_FILE_QUERY_INFO_NONE, NULL, &err);
}

FileInfo::FileInfo(GFileInfo *inf)
{
    m_info = inf;
}

FileInfo::~FileInfo()
{
}

QString FileInfo::fileName()
{
    return g_file_info_get_attribute_byte_string(m_info, G_FILE_ATTRIBUTE_STANDARD_NAME);
}

QString FileInfo::displayName()
{
    return QString::fromUtf8(g_file_info_get_attribute_string(m_info, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME));
}

QString FileInfo::baseName()
{
    // base name returns the name of the file up to (but not including) the first dot, e.g. archive.tar.gz => archive
    return displayName().split('.').first();
}

QString FileInfo::suffix()
{
    // returns the last suffix (from the last dot), e.g. archive.tar.gz => gz
    return displayName().split('.').last();
}

QString FileInfo::completeSuffix()
{
    // returns the complete suffix (from the first dot), e.g. archive.tar.gz => tar.gz
    QStringList splitted_name = displayName().split('.');
    // remove the baseName()
    splitted_name.removeFirst();
    return splitted_name.join(".");
}

QUrl FileInfo::absoluteUri()
{
    // shape : protocol://user:password@host:port/path/file.suffix
    return m_uri;
}

QString FileInfo::absoluteFilePath()
{
    return m_uri.path();
}

QString FileInfo::absolutePath()
{
    QString path = absoluteFilePath();
    if(path.endsWith('/')) // dir path with a trailing /
        path.truncate(path.length()-1);

    QStringList parts = path.split('/');
    parts.removeLast();
    return parts.join("/");
}

QString FileInfo::iconName()
{
    GIcon *ic = (GIcon*)g_file_info_get_attribute_object(m_info, G_FILE_ATTRIBUTE_STANDARD_ICON);
    if(ic)
        return g_icon_to_string(ic);
}

bool FileInfo::isFile()
{
    GFileType type = (GFileType)g_file_info_get_attribute_uint32(m_info, G_FILE_ATTRIBUTE_STANDARD_TYPE);
    return (type == G_FILE_TYPE_REGULAR);
}

bool FileInfo::isDir()
{
    GFileType type = (GFileType)g_file_info_get_attribute_uint32(m_info, G_FILE_ATTRIBUTE_STANDARD_TYPE);
    return (type == G_FILE_TYPE_DIRECTORY);
}

bool FileInfo::isSymlink()
{
    GFileType type = (GFileType)g_file_info_get_attribute_uint32(m_info, G_FILE_ATTRIBUTE_STANDARD_TYPE);
    return (type == G_FILE_TYPE_SYMBOLIC_LINK);
}

bool FileInfo::isReadable()
{
    return gbooleanToBool(g_file_info_get_attribute_boolean(m_info, G_FILE_ATTRIBUTE_ACCESS_CAN_READ));
}

bool FileInfo::isWritable()
{
    return gbooleanToBool(g_file_info_get_attribute_boolean(m_info, G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE));
}

bool FileInfo::isExecutable()
{
    return gbooleanToBool(g_file_info_get_attribute_boolean(m_info, G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE));
}

void FileInfo::setGFileInfo(GFileInfo *inf)
{
    m_info = inf;
}
