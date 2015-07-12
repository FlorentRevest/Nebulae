#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

// included further
class QTabWidget;
class FilesystemView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private slots:
    // for internal usage only ; opens a new tab with a new browsing area.
    // If no path is given, QDir::homePath is used
    void newTab(const QString &path = 0);

private slots:
    // the two following slots are called respectively when a tab is closed or
    // when the user switch to another tab, to update informations such as title...
    void m_closeTab(int);
    void m_tabSwitched(int);

    // those two ones are a bit mroe interesting, they are called when any of the
    // view present in the tabs change directory or selection, even not the current view
    void m_viewChangedDirectory(const QString &);
    void m_viewChangedSelection(QStringList);

    // used when the user click "Parent folder" on the toolbar. just call the cdUp()
    // method of the view in the current tab
    void m_cdup();

private:
    // The tab display
    QTabWidget *tabs;

    // internal method to found in which tab is contained a view. Note that i first thought
    // of just use the index, but it's quite hard with movable tabs
    int m_indexForView(FilesystemView *w);

    // store the selection to avoid useless code multiplications and so that we can send it
    // to some plugins are widget without having to get it back again from the view
    QStringList m_selection;
};

#endif // MAINWINDOW_H
