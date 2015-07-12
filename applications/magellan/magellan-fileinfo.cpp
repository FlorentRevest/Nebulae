#include "magellan-filebrowser.h"

#include <QtGui/QMessageBox>

// convenience fonctions to help Kommodity usage

// sort a list of file infos alphabetically
QList<FileInfo> FileBrowser::m_sort_info_list(QList<FileInfo> input)
{
    QList<FileInfo> output;

    // sort alphabetically
    // TODO: implement other sort methods


    while(!input.isEmpty())
    {
        FileInfo higher = input.first();
        int index = 0;
        for(int i = 0; i < input.count(); i++)
        {
            FileInfo inf = input.at(i);
            if(QString::localeAwareCompare(higher.getName(),inf.getName()) > 0) {
                higher = inf;
                index = i;
            }
        }
        output.append(higher);
        input.removeAt(index);
    }

    return output;
}

// list contents of a directory as a file info list
bool FileBrowser::m_directory_list(QUrl url, QList<FileInfo> *info)
{
    QList<FileInfo> dirs, files;

    // begin listing
    File file(url);
    if(file.isNull()) {
        QMessageBox::critical(this, "Error", "Null file", QMessageBox::Ok);
        return false;
    }

    Error error;
    FileEnumerator fileEnumerator = file.enumerateChildren("*", File::QueryInfoNorm, error);

    if(error.hasError()) {
        QMessageBox::critical(this, "Error", QString::fromUtf8(error.getMessage().toAscii()), QMessageBox::Ok);
        return false;
    }

    // split between files and dirs
    else
    {
        for (;;)
        {
            FileInfo fileInfo = fileEnumerator.nextFile(error);

            if(error.hasError()) {
                QMessageBox::critical(this, "error", QString::fromUtf8(error.getMessage().toAscii()), QMessageBox::Ok);
                return false;
            }

            if (fileInfo.isNull())
                break;  // finished

            if(fileInfo.getName().startsWith('.') && !m_display_hidden)
                continue;

            if(fileInfo.getContentType() == "inode/directory")
                dirs.append(fileInfo);
            else
                files.append(fileInfo);
        }

        fileEnumerator.close(error);
        if(error.hasError()) {
            QMessageBox::critical(this, "error", QString::fromUtf8(error.getMessage().toAscii()), QMessageBox::Ok);
            return false;
        }
    }

    // sort both lists
   info->append(m_sort_info_list(dirs));
   info->append(m_sort_info_list(files));
   return true;
}

// use various methods to get the icon of a FileInfo
QIcon FileBrowser::m_file_info_icon(FileInfo inf)
{
    Icon *in = inf.getIcon();

    // try thumbnail
    QString thumbnail = QString(inf.getAttributeByteString("thumbnail::path"));
    if(!thumbnail.isEmpty())
        return QIcon(thumbnail);

    // try theme icon
    ThemedIcon *ic = dynamic_cast<ThemedIcon*>(in);
    foreach(QString name, ic->getNames())
    {
        QIcon ret = QIcon::fromTheme(name);
        if(!ret.isNull())
            return ret;
    }

    // not found... return blank file
    return QIcon::fromTheme("text-plain");
}

