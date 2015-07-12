#ifndef MAGELLANMAINWINDOW_H
#define MAGELLANMAINWINDOW_H

#include <QtCore/QUrl>

#include <QtGui/QLineEdit>
#include <QtGui/QTabWidget>
#include <QtGui/QMainWindow>

#include "magellan-filebrowser.h"
#include "magellan-toolbar.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    // connected to signal from UI elements that request
    // an URL change or a tab opening
    void m_url_change_requested(QUrl url);
    void m_new_tab_requested(QUrl url = QUrl(), bool activate = true);
    void m_close_tab_requested(int index);
    void m_current_tab_changed(int index);

    // various UI actions
    void m_back_requested();
    void m_next_requested();
    void m_top_requested();
    void m_view_mode_changed(QListWidget::ViewMode);
    void m_options_requested();

    // the URI of a widget changed
    void m_widget_changed_url(QUrl);

private:
    // the main view uses a tabbed display
    QTabWidget *m_tab_widget;

    // functions for tab management
    // returns the FileBrowser widget in the current tab
    FileBrowser *m_get_widget();

    // opens a new browsing tab and return it's index
    int m_new_tab(QUrl url = QUrl());

    Toolbar *m_tb;
};

#endif // MAGELLANMAINWINDOW_H
