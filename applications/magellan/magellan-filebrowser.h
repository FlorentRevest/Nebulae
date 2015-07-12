#ifndef MAGELLANFILEBROWSER_H
#define MAGELLANFILEBROWSER_H

#include <QtCore/QUrl>
#include <QtCore/QFileSystemWatcher>

#include <QtGui/QWidget>
#include <QtGui/QListWidget>
#include <QtGui/QUndoStack>
#include <QtGui/QUndoCommand>

#include "Kommodity/GIO/File"
#include "Kommodity/GIO/FileInfo"
#include "Kommodity/GIO/ThemedIcon"
#include "Kommodity/GIO/FileEnumerator"
using namespace Kommodity::GIO;

class FileBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit FileBrowser(QWidget *parent = 0);

    // returns the current URL/title
    QUrl url();
    QString title();

    // returns if the undo stack can undo/redo
    bool canUndo();
    bool canRedo();

signals:
    // emitted when the loading of an URL is finished
    void urlChanged(QUrl url);

public slots:
    // changes the current URL and loads it
    void setUrl(QUrl new_url, bool stack = true);
    void cdUp();
    void back();
    void next();
    void setViewMode(QListWidget::ViewMode, bool refresh = true);

private slots:
    void m_item_clicked(QListWidgetItem *it);
    void m_item_double_clicked(QListWidgetItem *it);
    void m_file_system_changed(QString);

private:
    // the current URL
    QUrl m_url;

    // main display widget
    QListWidget *m_display;
    QHash<QListWidgetItem*,FileInfo> m_item_info;

    // options
    bool m_display_hidden;
    bool m_lock;

    // internal methods
    QList<FileInfo> m_sort_info_list(QList<FileInfo>);
    bool m_directory_list(QUrl, QList<FileInfo>*);
    QIcon m_file_info_icon(FileInfo);

    // a stack containing undo actions
    QUndoStack *m_undo_stack;

    // when in local, watch the FS for changes
    QFileSystemWatcher *m_watcher;

protected:
    virtual void resizeEvent(QResizeEvent *) {
        m_display->resize(size());
    }
};

class UndoAction : public QUndoCommand
{

public:
    UndoAction(FileBrowser *f_b, QUrl current, QUrl previous) : m_fb(f_b), m_current(current), m_previous(previous) {}

private:
    FileBrowser *m_fb;
    QUrl m_current, m_previous;

protected:
    virtual void undo() {
        m_fb->setUrl(m_previous, false);
    }
    virtual void redo() {
        m_fb->setUrl(m_current, false);
    }

};

#endif // MAGELLANFILEBROWSER_H
