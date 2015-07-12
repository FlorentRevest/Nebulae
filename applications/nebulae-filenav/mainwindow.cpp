#include "mainwindow.h"
#include "filesystemview.h"

#include <QtGui/QToolBar>
#include <QtGui/QTabWidget>
#include <QtGui/QPushButton>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // set some window flags
    move(50,50);
    resize(640,400);
    setWindowTitle("File browser");

    // we're gonna use a tabbed view
    tabs = new QTabWidget(this);
    tabs->setMovable(true);             // the user like to move the tabs; TODO: is it really a good idea ? It can modify the index...
    tabs->setTabsClosable(true);        // this adds a little button to close a tab on it
    setCentralWidget(tabs);

    // we have to konw when the user siwth to another tab, to adapt the window title, the preview area, etc
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(m_tabSwitched(int)));
    // we also must know when the user click on the little "close" button. note: QTabWidget doesn't close the tab
    // we have to do it ourselves
    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(m_closeTab(int)));

    // set a corner button to the tab view to open new tabs
    QPushButton *tabs_corner_button = new QPushButton(QIcon::fromTheme("add"), "", this);
    tabs_corner_button->setToolTip(tr("Opens a new browsing tab"));
    connect(tabs_corner_button, SIGNAL(clicked()), this, SLOT(newTab()));
    tabs->setCornerWidget(tabs_corner_button, Qt::TopRightCorner);

    // let's build some toolbar
    QToolBar *tb = addToolBar(tr("Navigation"));
    // it has to stay where it is :|
    tb->setAllowedAreas(Qt::TopToolBarArea);
    tb->setMovable(false);
    tb->setFloatable(false);
    // add a single action to get to the parent directory. will be removed when we will have the address bar
    tb->addAction(QIcon::fromTheme("go-up"), tr("Parent folder"), this, SLOT(m_cdup()));

    // by default, we open a single path, the home
    // TODO: use command line parameters to open the specified location
    newTab("file:///home/leo");
}

void MainWindow::newTab(const QString &basePath)
{
    // if no path is given, use the home, as always...
    QString path = basePath.isEmpty() ? QUrl::fromLocalFile(QDir::homePath()).toString() : basePath;

    // create a new view widget and place it in the tab display
    FilesystemView *w = new FilesystemView(path, this);
    connect(w, SIGNAL(newTabRequested(QString)), this, SLOT(newTab(QString)));
    connect(w, SIGNAL(directoryChanged(QString)), this, SLOT(m_viewChangedDirectory(QString)));
    connect(w, SIGNAL(selectionChanged(QStringList)), this, SLOT(m_viewChangedSelection(QStringList)));
    tabs->setCurrentIndex(tabs->addTab(w, path.split("/").last()));
}

void MainWindow::m_cdup()
{
    FilesystemView *w = qobject_cast<FilesystemView*>(tabs->currentWidget());
    w->cdUp();
}

void MainWindow::m_closeTab(int i)
{
    // the user requested a tab to be closed by clickin the little button :
    tabs->removeTab(i);

    // don't let an empty area
    if(tabs->count() < 1)
        newTab();
}

void MainWindow::m_tabSwitched(int index)
{
    // the given index is the new tab's one. Get the corresponding view
    FilesystemView *w = qobject_cast<FilesystemView*>(tabs->widget(index));

    // update the title and the selection
    setWindowTitle(QString(w->directory().encodedPath()).split("/").last());
    m_selection = w->selection();
}

void MainWindow::m_viewChangedDirectory(const QString &directory)
{
    // some view changed directory
    FilesystemView *w = qobject_cast<FilesystemView*>(sender());
    if(!w)
        return;

    // get the index of the tab containing the view
    int index = m_indexForView(w);
    if(index == -1)
        return;

    // update the title of its tab
    tabs->setTabText(index, directory.split("/").last());

    // if this view is the current one, we have to change de window title too !
    if(index == tabs->currentIndex())
        setWindowTitle(directory.split("/").last());
}

void MainWindow::m_viewChangedSelection(QStringList selection)
{
    // as above, but with the slectioned files list
    FilesystemView *w = qobject_cast<FilesystemView*>(sender());
    if(!w)
        return;

    // get the index of the tab containing the view
    int index = m_indexForView(w);
    if(index == -1)
        return;

    // nothing to do for the tab right now, just update the selection if the view is the current one
    if(index == tabs->currentIndex())
        m_selection = selection;
}

int MainWindow::m_indexForView(FilesystemView *w)
{
    // just browse the tabs until we reach the good one
    for(int index = 0; index < tabs->count(); index++)
    {
        if(qobject_cast<FilesystemView*>(tabs->widget(index)) == w)
            return index;
    }

    // sorry, we didn't found anything :(
    return -1;
}

// i think it do is clear enough :)
