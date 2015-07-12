#ifndef FILESYSTEMVIEW_H
#define FILESYSTEMVIEW_H

#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileSystemWatcher>

#include <QtGui/QMenu>
#include <QtGui/QWidget>
#include <QtGui/QListWidget>

// Those class are just defined here to avoid multiple and multiple includes
// Their implementation is included in the .cpp
class QSettings;
class FileIconProvider;
class FilesystemView : public QWidget
{
    enum ViewMode {
        IconMode,
        ListMode
    };

    Q_OBJECT
public:
    FilesystemView(const QUrl &baseUri = QUrl(), QWidget *parent = 0);

    // the following methods are used to get some properties of the view
    QUrl directory();        // returns the current value of m_current_directory
    QStringList selection();    // returns a list of the selected files (absolute paths)

signals:
    // emitted to notify the parent widget that the loading of a new directory is done
    void directoryChanged(const QUrl &directory);

    // emitted when the selection changed (used by some plugins to now what is selected
    void selectionChanged(QStringList selectedFiles);

    // emitted to tell the parent widget the user requested a new tab to be opened with
    // the given base path
    void newTabRequested(const QString &directory);

public slots:
    void setDirectory(const QUrl &directory);
    void cd(const QString &directory);
    void cdUp();
    void refresh();

private slots:
    // private actions that populate the QMenu and display it depending of where did
    // the user right-clicked
    void m_displayContextMenu(QPoint);
    void m_displayContextMenuOnVoid(QPoint);
    void m_displayContextMenuOnFile(QPoint);
    void m_displayContextMenuOnDir(QPoint);
    void m_displayContextMenuWithSelection(QPoint);

    // contextual actions called by the menu that does simple file management operations
    // they get the path(s) of the selected file(s) by reading the property "FilePath" of
    // the sender action. the m_selected_* use a QStringList to act on many files, whereas
    // the m_file_* and m_dir_* actions use a single QString file path.
    // the m_act_* that are displayed when the user right click without selection on a void
    // area should read the address of the current directory from this property

    // The slot below is called when a item is double ckicked in the view. For a dir, it's opened
    // (the cd() method is called), and for a file, it's opened with the associated default app
    void m_doubleClick(QListWidgetItem*);

    // this one is internaly called when the selection changed and makes a proper QStringList that
    // can then be sended by selectionChaned();
    void m_selectionChanged(QItemSelection selected, QItemSelection deselected);

    // called when the QFileSystemWatcher notifies us of an event
    void m_fs_changed(const QString &path);

    // this one is similar to the one just above, but for the watcher that watch the config file
    void m_config_changed(const QString &path);

private:
    QWidget *m_parent;

    // Absolute URL (protocol://user:passwd@host:port/path/file.suffix) path of the currently viewed directory
    QUrl m_current_directory;

    // The display widget
    QListWidget *m_list;

    // Used to auto-reload display when there's a new/deleted file, or when the config file is modified
    QFileSystemWatcher *m_fswatcher, *m_config_watcher;

    // Used to get the icon of a dir/file
    FileIconProvider *m_pvd;

    // Use to display any context menu
    QMenu *m_context;

    // The main application Settings
    QSettings *m_settings;

    // The current selection (stored to avoid useless code multiplications)
    QStringList m_selection;

    // low level methods for file manipulation (see actions.cpp)
    bool m_delete(QFileInfoList);
    bool m_copy(QFileInfoList);
    bool m_copy_file(const QString&, const QString&);
    bool m_copy_dir(const QString&, const QString&);
    bool m_paste(QFileInfoList, const QString&);
};

#endif // FILESYSTEMVIEW_H
