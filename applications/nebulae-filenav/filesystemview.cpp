#include "filesystemview.h"
#include "listwidgetitem.h"
#include "fileiconprovider.h"

#include <QtCore/QUrl>
#include <QtCore/QProcess>
#include <QtCore/QSettings>

#include <QtGui/QGridLayout>
#include <QtGui/QDesktopServices>

#include <QtDebug>

#include "nebulae-io.h"

FilesystemView::FilesystemView(const QUrl &baseUri, QWidget *parent) : QWidget(parent), m_parent(parent)
{
    // initialize the main display widget and place it at the center
    m_list = new QListWidget(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_list);
    setLayout(layout);

    // initialize the main settings
    m_settings = new QSettings(QDir::homePath() + "/.config/Magellan/Magellan.conf", QSettings::NativeFormat);

    // initialize the file system watchers and the icon provider but leave them unused for the moment
    m_fswatcher = new QFileSystemWatcher(this);
    m_config_watcher = new QFileSystemWatcher(this);
    m_pvd = new FileIconProvider();

    // program the file system watcher to warn us when the config file has been modified
    m_config_watcher->addPath(QDir::homePath() + "/.config/Magellan/Magellan.conf");
    connect(m_config_watcher, SIGNAL(fileChanged(QString)), this, SLOT(m_config_changed(QString)));

    // if no initial path is given to display, use the user's home instead
    m_current_directory = baseUri.isEmpty() ? QUrl::fromLocalFile(QDir::homePath()) : baseUri;

    m_list->setMovement(QListWidget::Snap);                         // the user can move the items, but they are stucked into a grid
    m_list->setSelectionMode(QAbstractItemView::ExtendedSelection); // allow to select multiple items at the same time
    m_list->setResizeMode(QListWidget::Adjust);                     // items are laid out again each time the user resizes the window, to full the width
    m_list->setWordWrap(true);                                      // long file names can use multiple lines
    connect(m_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(m_doubleClick(QListWidgetItem*)));
    connect(m_list->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(m_selectionChanged(QItemSelection,QItemSelection)));  // allow to know when the selection change

    // initialize the menu of the list display
    m_list->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_list, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(m_displayContextMenu(QPoint)));
    m_context = new QMenu(this);

    // we're ready to display
    refresh();
}

void FilesystemView::refresh()
{
    qDebug() << QUrl::fromEncoded(m_current_directory.toString().toAscii());
    // by security, we disable the watchers's warnings while the refresh procedure
    m_fswatcher->disconnect(this, SLOT(m_fs_changed(QString)));
    m_config_watcher->disconnect(this, SLOT(m_config_changed(QString)));

    // if we put some variable options here, we are sure that we can update them by simply calling the refresh() method, as example each time the config file is modified
    if(m_settings->value("Display/ViewMode", "Icon").toString() == "List")
    {
        // read the size of an icon from the settings. It's a different property than the one used by the icon display
        m_list->setViewMode(QListWidget::ListMode);
        m_list->setIconSize(QSize(m_settings->value("Display/ListIconSize",32).toInt(),m_settings->value("Display/ListIconSize",32).toInt()));
    }
    else
    {
        // size of the icons is stored in the config, as above
        // TODO: find a grid size, that is not stored in the config, but depends of the icon size
        m_list->setViewMode(QListWidget::IconMode);
        m_list->setIconSize(QSize(m_settings->value("Display/IconSize",48).toInt(),m_settings->value("Display/IconSize",48).toInt()));
        m_list->setGridSize(QSize(128,96));
    }
    m_pvd->setIconSize(m_list->iconSize());

    // first of all, check the directory the user asked for does exists
    if(!Dir(m_current_directory.toString()).exists())
    {
        cd(QDir::homePath());
        return;
    }

    // clear the view
    m_list->clear();

    // explore and displays the containts of the directory
    Dir dir(m_current_directory.toString());

    // does we have to display hidden files ?
    //QFlags<QDir::Filter> filter = QDir::AllEntries | QDir::NoDotAndDotDot;
    //if(m_settings->value("Display/HiddenFiles", "No").toString() == "Yes")
    //    filter |= QDir::Hidden;

    FileInfoList lst = dir.entryInfoList();
    foreach(FileInfo inf, lst)
    {
        ListWidgetItem *it = new ListWidgetItem(inf.isDir(), inf.absoluteUri(), m_list);
        it->setText(inf.displayName());
        it->setIcon(m_pvd->icon(inf));

        // little function for a better integration of launchers :) (.desktop)
        if(inf.fileName().split(".").last() == "desktop")
        {
            QSettings s(inf.absoluteFilePath(), QSettings::NativeFormat);
            s.beginGroup("Desktop Entry");
            it->setText(s.value("Name[" + QLocale::system().name().section('_',0,0) + "]", s.value("Name", inf.fileName()).toString()).toString());
            it->setIcon(QIcon::fromTheme(s.value("Icon", "application-default-icon").toString(), QIcon::fromTheme("application-default-icon")));
            s.endGroup();
        }

        m_list->addItem(it);
    }

    // emit a signal to notify we finished to load the directory
    emit directoryChanged(m_current_directory);

    // reinitialize the selection (if not done)
    m_selection.clear();
    emit selectionChanged(m_selection);

    // program the file system watcher to tells us when a file is created/deleted/renamed/whatever in this directory, and re-enable it
    //m_fswatcher->removePaths(m_fswatcher->directories());   // reinitialize it
    //m_fswatcher->addPath(m_current_directory);
    connect(m_fswatcher, SIGNAL(directoryChanged(QString)), this, SLOT(m_fs_changed(QString)));

    // also re-enable the one taht watches the config file
    connect(m_config_watcher, SIGNAL(fileChanged(QString)), this, SLOT(m_config_changed(QString)));
}

void FilesystemView::setDirectory(const QUrl &directory)
{
    // Sets the current direcotry, but doesn't refresh so view, so the modifications will be
    // seen once the view will be refreshed
    m_current_directory = directory;
}

void FilesystemView::cd(const QString &directory)
{
    // works as the cd command of a shell, changes the current directory (keeps the same host, etc, and refresh
    m_current_directory.setPath(directory);
    refresh();
}

void FilesystemView::cdUp()
{
    // uses cd() to get to the parent directory of the one we are browsing
    QString path = QUrl(m_current_directory).path();

    // don't get up to the filesystem root
    if(path == "/" || path.isEmpty())
        return;

    // cd !
    QStringList prts = path.split("/");
    prts.removeLast();
    prts.removeAll("");
    if(prts.count() == 0)
        cd("/"); // fix a bug when we try to use this function from "/home" ou "/usr", that loaded a empty directory
    cd("/" + prts.join("/"));
}

void FilesystemView::m_doubleClick(QListWidgetItem *it)
{
    // convert into our own ListWidgetItem class so that we can get it's path
    ListWidgetItem *item = static_cast<ListWidgetItem*>(it);
    if(!item)
        return;
    QString path = item->filePath().path();

    // if it's a dir, "open" it, e.g. switch the view to it
    if(item->isDir())
        cd(path);
    else // if it's a file, use the default app to open it
    {
        if(path.split(".").last() == "desktop") // it's a launcher !
        {
            // Read the Exec value and launch it with a QProcess
            QSettings s(path, QSettings::NativeFormat);
            s.beginGroup("Desktop Entry");
            QProcess p(this);
            p.startDetached(s.value("Exec").toString().remove("%u", Qt::CaseInsensitive));
            s.endGroup();
        }
        else // if not... let Qt find the best app for us :)
            QDesktopServices::openUrl(QUrl(path));
    }
}

void FilesystemView::m_selectionChanged(QItemSelection, QItemSelection)
{
    QStringList sel;
    // The parameters give us what is newly (de)selected, this is not what want, we want
    // the full selection. We so use only the signal "as it", as a ring bell, and get the
    // full selection with a mroe low level method :
    foreach(QListWidgetItem *item, m_list->selectedItems())
    {
        // convert into our own ListWidgetItem object to get the path (i love this method *_*)
        ListWidgetItem *it = static_cast<ListWidgetItem*>(item);
        sel.append(it->filePath().path());
    }

    // store the selection and emit a signal
    m_selection = sel;
    emit selectionChanged(sel);
}

void FilesystemView::m_fs_changed(const QString &path)
{
    // some file has been created/deleted/modified... juste update the view :)
    //if(path == m_current_directory)
    //    refresh();
}

void FilesystemView::m_config_changed(const QString &)
{
    // Somebody edited the configuration file, refresh the view to use the newer options ! :D
    qDebug() << "file modified...";
    refresh();
}

// utility methods to get some properties

QUrl FilesystemView::directory()
{
    // does it really have to be commented, huh ? :)
    return m_current_directory;
}

QStringList FilesystemView::selection()
{
    // as above, hehe
    return m_selection;
}
