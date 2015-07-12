#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QHash>

#include <QtGui/QMainWindow>

class QLabel;
class QSettings;
class QListWidgetItem;
class QListWidget;
class DesktopEntry;
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void m_list_item_clicked(QListWidgetItem*);
    void m_set_default();
    void m_edit_program();
    void m_editor_finished();

private:
    // cached list of entries
    QList<DesktopEntry *> m_entries;
    QHash<QListWidgetItem*,DesktopEntry*> m_entries_items;
    QListWidgetItem *m_current_item;
    QListWidget *m_list;

    // display labels
    QLabel *m_ic_label, *m_name_label, *m_defaults_label;
    QDockWidget *m_side_doc;
    QWidget *m_side_widget;

    // files that contains the defaults apps info
    QSettings *m_global, *m_local;
};

#endif // MAINWINDOW_H
