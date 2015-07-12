#include "fileiconprovider.h"
#include "nebulae/nebulae-basedir.h"
#include "nebulae/nebulae-mimetypeinfo.h"
#include "nebulae/nebulae-desktopentry.h"

#include <QtCore/QObject>
#include <QtDebug>

FileIconProvider::FileIconProvider() : QFileIconProvider()
{
    setIconSize(QSize(48,48));
}

void FileIconProvider::setIconSize(QSize s)
{
    m_size = s;
}

QSize FileIconProvider::iconSize()
{
    return m_size;
}

QIcon FileIconProvider::icon(FileInfo info)
{
    if(info.isDir())
    {
        // returns friendly icons for the common dirss
        QString path = info.absoluteFilePath();
        if(path == HOME + "/" + QObject::tr("Pictures"))
            return QIcon::fromTheme("folder-pictures");
        else if(path == HOME + "/" + QObject::tr("Music"))
            return QIcon::fromTheme("folder-music");
        else if(path == HOME + "/" + QObject::tr("Documents"))
            return QIcon::fromTheme("folder-documents");
        else if(path == HOME + "/" + QObject::tr("Downloads"))
            return QIcon::fromTheme("folder-downloads");
        else if(path == HOME + "/" + QObject::tr("Videos"))
            return QIcon::fromTheme("folder-videos");
        else if(path == HOME)
            return QIcon::fromTheme("folder-home");
    }
/*
    else if(nApp->mimeTypeDatabase()->fileType(info).name().startsWith("image/"))
    {
        // the file is a picture, load a preview
        QPixmap pix;
        if(pix.load(info.absoluteFilePath()))
        {
            pix = pix.scaled(m_size, Qt::KeepAspectRatio, Qt::FastTransformation);
            return QIcon(pix);
        }
    }
*/
    else if(info.completeSuffix() == "desktop")
    {
        // icons of the .desktop files
        DesktopEntry d(info.absoluteFilePath());
        return d.icon();
    }

    // if not of the above types...
    QStringList icons = info.iconName().split(" ");
    foreach(QString icon, icons)
    {
        QIcon ret = QIcon::fromTheme(icon);
        if(!ret.isNull())
            return ret;
    }
    return QIcon();
}
