#include "magellan-filebrowser.h"

#include <QtGui/QMessageBox>
#include <QtGui/QDesktopServices>
#include <QtDebug>

FileBrowser::FileBrowser(QWidget *parent) :
    QWidget(parent)
{
    // initialize the display and its various properties
    m_display = new QListWidget(this);
    setViewMode(QListWidget::IconMode, false);
    connect(m_display, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(m_item_clicked(QListWidgetItem*)));
    connect(m_display, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(m_item_double_clicked(QListWidgetItem*)));

    // initialize the history stack
    m_undo_stack = new QUndoStack(this);

    // init the FS watcher, will work only in local
    m_watcher = new QFileSystemWatcher(this);

    m_lock = false;
}

// returns the current URL
QUrl FileBrowser::url()
{
    return m_url;
}

// returns the last part of the URL, that should be used as a title
QString FileBrowser::title()
{
    Error error;
    return QString::fromUtf8(File(m_url).queryInfo("*", File::QueryInfoNorm, error).getDisplayName().toAscii());
}

bool FileBrowser::canUndo()
{
    return m_undo_stack->canUndo();
}

bool FileBrowser::canRedo()
{
    return m_undo_stack->canRedo();
}

void FileBrowser::setUrl(QUrl new_url, bool stack)
{
    qDebug() << new_url << " received [FileBrowser]";

    // the lock prevent two instances of this slot to run simultanely
    // that causes random crashes
    if(m_lock)
        return;
    m_lock = true;

    QList<FileInfo> infs;
    if(!m_directory_list(new_url, &infs)) {
        m_lock = false;
        return;
    }

    if(stack && (!new_url.isEmpty()) && (!m_url.isEmpty()) && (new_url != m_url))
    {
        // stack it
        UndoAction *act = new UndoAction(this, new_url, m_url);
        m_undo_stack->push(act);
    }

    // disconnect the FS watcher
    foreach(QString path, m_watcher->directories())
    {
        m_watcher->removePath(path);
    }
    disconnect(m_watcher, SIGNAL(fileChanged(QString)), this, SLOT(m_file_system_changed(QString)));

    m_url = new_url;
    m_display->clear();
    m_item_info.clear();

    foreach(FileInfo fileInfo, infs)
    {
        // add the file to the view
        QListWidgetItem *it = new QListWidgetItem(m_display);
        it->setText(QString::fromUtf8(fileInfo.getDisplayName().toAscii()));
        qDebug() << it->text();
        it->setIcon(m_file_info_icon(fileInfo));
        m_item_info.insert(it, fileInfo);
    }

    // if in local, re enabled the watcher
    if(m_url.scheme() == "file")
    {
        m_watcher->addPath(m_url.path());
        connect(m_watcher, SIGNAL(directoryChanged(QString)), this, SLOT(m_file_system_changed(QString)));
    }

    //if(FileInfo(new_url))

    qDebug() << new_url << " emitted [FileBrowser]";
    emit urlChanged(m_url);
    m_lock = false;
}

void FileBrowser::cdUp()
{
    // goes to the parent folder
    QString path = m_url.path();
    if(path != "/")
    {
        while(path.endsWith("/"))
            path.truncate(path.length()-1);
        QStringList parts = path.split('/');
        parts.removeAll("");
        if(parts.count() > 1)
        {
            parts.removeLast();
            path = "/" + parts.join("/");
        }
        else
            path = "/";
    }
    m_url.setPath(path);
    setUrl(m_url);
}

void FileBrowser::back()
{
    m_undo_stack->undo();
}

void FileBrowser::next()
{
    m_undo_stack->redo();
}

void FileBrowser::setViewMode(QListWidget::ViewMode mode, bool refresh)
{
    if(mode == QListWidget::IconMode)
    {
        m_display->setViewMode(QListWidget::IconMode);
        m_display->setIconSize(QSize(48,48));
        m_display->setGridSize(QSize(128,128));
        m_display->setUniformItemSizes(false);
        m_display->setMovement(QListWidget::Snap);
    }
    else if(mode == QListWidget::ListMode)
    {
        m_display->setViewMode(QListWidget::ListMode);
        m_display->setIconSize(QSize(24,24));
        m_display->setGridSize(QSize(24,24));
        m_display->setUniformItemSizes(true);
        m_display->setMovement(QListWidget::Static);
    }

    if(mode == QListWidget::IconMode)
    m_display->setResizeMode(QListWidget::Adjust);
    m_display->setSelectionMode(QListWidget::ExtendedSelection);

    if(refresh)
        setUrl(m_url, false);
}

void FileBrowser::m_item_double_clicked(QListWidgetItem *it)
{
    FileInfo inf = m_item_info.value(it);

    if(inf.getContentType() == "inode/directory")
        setUrl(QUrl(m_url.toString() + (m_url.path().endsWith("/") ? QString() : "/") + QString::fromUtf8(inf.getDisplayName().toAscii()) + "/"));
    else
        QDesktopServices::openUrl(QUrl(m_url.toString() + (m_url.path().endsWith("/") ? QString() : "/") + QString::fromUtf8(inf.getDisplayName().toAscii())));
}

void FileBrowser::m_item_clicked(QListWidgetItem *)
{

}

void FileBrowser::m_file_system_changed(QString path)
{
    // check we are still viewing this path... :p
    if(path == m_url.path())
        setUrl(m_url, false); // refresh
}
