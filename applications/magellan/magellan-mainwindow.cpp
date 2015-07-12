#include "magellan-mainwindow.h"
#include "magellan-devicemanager.h"

#include <QtCore/QDir>

#include <QtGui/QToolBar>
#include <QtGui/QDockWidget>
#include <QtGui/QPushButton>

#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QDockWidget *m_left_dock = new QDockWidget(tr("Devices"), this);
    addDockWidget(Qt::LeftDockWidgetArea, m_left_dock);
    m_left_dock->setFeatures(QDockWidget::NoDockWidgetFeatures);

    // initialize the device manager
    DeviceManager *dev_mgr = new DeviceManager(this);
    m_left_dock->setWidget(dev_mgr);
    connect(dev_mgr, SIGNAL(urlChanged(QUrl)), this, SLOT(m_url_change_requested(QUrl)));

    m_tb = new Toolbar(this);
    addToolBar(Qt::TopToolBarArea, m_tb);
    m_tb->setMovable(false);
    m_tb->setFloatable(false);
    m_tb->setAllowedAreas(Qt::TopToolBarArea);

    connect(m_tb, SIGNAL(urlChanged(QUrl)), this, SLOT(m_url_change_requested(QUrl)));
    connect(m_tb, SIGNAL(backClicked()), this, SLOT(m_back_requested()));
    connect(m_tb, SIGNAL(nextClicked()), this, SLOT(m_next_requested()));
    connect(m_tb, SIGNAL(topClicked()), this, SLOT(m_top_requested()));
    connect(m_tb, SIGNAL(viewModeChanged(QListWidget::ViewMode)), this, SLOT(m_view_mode_changed(QListWidget::ViewMode)));

    // initialize tab display
    m_tab_widget = new QTabWidget(this);
    setCentralWidget(m_tab_widget);
    QPushButton *new_tab_btn = new QPushButton(QIcon::fromTheme("tab-new"), QString(), this);
    new_tab_btn->setToolTip(tr("New tab"));
    new_tab_btn->setFlat(true);
    m_tab_widget->setCornerWidget(new_tab_btn, Qt::TopLeftCorner);
    m_tab_widget->setTabsClosable(true);
    connect(m_tab_widget, SIGNAL(currentChanged(int)), this, SLOT(m_current_tab_changed(int)));
    connect(m_tab_widget, SIGNAL(tabCloseRequested(int)), this, SLOT(m_close_tab_requested(int)));
    connect(new_tab_btn, SIGNAL(clicked()), this, SLOT(m_new_tab_requested()));

    m_new_tab(QUrl("network:///"));
}


void MainWindow::m_url_change_requested(QUrl url)
{
    qDebug() << url << " received [main window]";
    // an UI element requested the current URL to be changed
    // forward it to the current widget
    FileBrowser *f_b = m_get_widget();
    if(f_b)
        f_b->setUrl(url);
}

void MainWindow::m_new_tab_requested(QUrl url, bool activate)
{
    url = (url.isEmpty()) ? QUrl("file://" + QDir::homePath()) : url;
    // an UI element requested this URL to be opened in a new tab
    // if activate, this tab should become the active one
    int index = m_new_tab(url);
    if(activate)
        m_tab_widget->setCurrentIndex(index);
}

void MainWindow::m_close_tab_requested(int index)
{
    // close the requested tab
    m_tab_widget->removeTab(index);
}

void MainWindow::m_current_tab_changed(int)
{
    // the current tab has changed, change the title of the window
    FileBrowser *f_b = m_get_widget();
    if(f_b)
    {
        QString title = f_b->title();
        setWindowTitle(title);

        m_tb->setUrl(f_b->url());
        m_tb->setUndoRedoEnabled(f_b->canUndo(), f_b->canRedo());
    }
}

void MainWindow::m_back_requested()
{
    FileBrowser *f_b = m_get_widget();
    if(f_b)
        f_b->back();
}

void MainWindow::m_next_requested()
{
    FileBrowser *f_b = m_get_widget();
    if(f_b)
        f_b->next();
}

void MainWindow::m_top_requested()
{
    FileBrowser *f_b = m_get_widget();
    if(f_b)
        f_b->cdUp();
}

void MainWindow::m_view_mode_changed(QListWidget::ViewMode mode)
{
    FileBrowser *f_b = m_get_widget();
    if(f_b)
        f_b->setViewMode(mode);
}

void MainWindow::m_options_requested()
{
    // m_open_options(); IT'S TO BE IMPLEMENTED TOO ! :o *sigh*
}

void MainWindow::m_widget_changed_url(QUrl url)
{
    qDebug() << url << " received from browser [main window]";

    // one of the widgets change url. update the title of it's tab
    FileBrowser *f_b = qobject_cast<FileBrowser*>(sender());
    if(f_b)
    {
        QString title = f_b->title();
        m_tab_widget->setTabText(m_tab_widget->indexOf(f_b), title);
        m_tab_widget->setTabToolTip(m_tab_widget->indexOf(f_b), url.toString());

        // check if it's the current one, if yes, update the window title
        if(f_b == m_get_widget())
        {
            m_tb->setUrl(url);
            setWindowTitle(title);

            // update previous/back buttons
            m_tb->setUndoRedoEnabled(f_b->canUndo(), f_b->canRedo());
        }
    }
}

FileBrowser *MainWindow::m_get_widget()
{
    return qobject_cast<FileBrowser*>(m_tab_widget->currentWidget());
}

int MainWindow::m_new_tab(QUrl url)
{
    url = (url == QUrl()) ? "file://" + QDir::homePath() : url;

    FileBrowser *f_b = new FileBrowser(this);
    connect(f_b, SIGNAL(urlChanged(QUrl)), this, SLOT(m_widget_changed_url(QUrl)));
    int index = m_tab_widget->addTab(f_b, QString());
    f_b->setUrl(url);

    return index;
}
