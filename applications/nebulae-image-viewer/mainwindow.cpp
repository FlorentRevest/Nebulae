#include "mainwindow.h"
#include "imageviewer.h"

#include <QtCore/QDirIterator>

#include <QtGui/QToolBar>
#include <QtGui/QBoxLayout>
#include <QtGui/QPushButton>

void *MainWindow::m_build_toolbar()
{
    m_toolbar = new QWidget(this);

    // this label is used to display the current dir
    m_dir_label = new QLabel(this);

    // three buttons to modify the picture display
    QPushButton *btn_zoomin = new QPushButton(QIcon::fromTheme("zoom-in"), "", this);
    QPushButton *btn_zoomout = new QPushButton(QIcon::fromTheme("zoom-out"), "", this);
    QPushButton *btn_zoomfit = new QPushButton(QIcon::fromTheme("zoom-fit"), "", this);

    // set help tips
    btn_zoomin->setToolTip(tr("Zoom in"));
    btn_zoomout->setToolTip(tr("Zoom out"));
    btn_zoomfit->setToolTip(tr("Adjust the window size"));

    connect(btn_zoomin, SIGNAL(clicked()), m_viewer, SLOT(zoomIn()));
    connect(btn_zoomout, SIGNAL(clicked()), m_viewer, SLOT(zoomOut()));
    connect(btn_zoomfit, SIGNAL(clicked()), m_viewer, SLOT(adjust()));

    // add the widgets in the bar with the correct alignement
    QHBoxLayout *tb_layout = new QHBoxLayout(this);
    tb_layout->addWidget(m_dir_label);
    tb_layout->addStretch();
    tb_layout->addWidget(btn_zoomin);
    tb_layout->addWidget(btn_zoomout);
    tb_layout->addWidget(btn_zoomfit);
    tb_layout->setContentsMargins(0,0,0,0);
    m_toolbar->setLayout(tb_layout);
}

MainWindow::MainWindow(QString dir_path, QString file_name, QWidget *parent) : QMainWindow(parent)
{
    // initlialize widgets, first, the display
    m_viewer = new ImageViewer(this);

    // the bar
    m_build_toolbar();

    // the list of pictures
    // we are gonna use a QListWidget with some few options for this control
    m_list = new QListWidget(this);
    m_list->setFlow(QListWidget::TopToBottom);          // displays items horizontally
    m_list->setViewMode(QListWidget::IconMode);         // use icons instead of list
    m_list->setGridSize(QSize(100,100));                // fixed grid size
    m_list->setIconSize(QSize(100,100));                // same icon size
    m_list->setMovement(QListWidget::Static);           // don't allow the user to move the items
    m_list->setResizeMode(QListWidget::Adjust);         // adjust the items when resizing
    m_list->setFixedHeight(120);                        // fix the size of the widget
    m_list->setMinimumHeight(120);
    m_list->setMaximumHeight(120);
    m_list->setSpacing(0);


    // populates it with the current directory
    // check the given directory is valid
    if(!QDir(dir_path).exists()) // if not, use a default folder
        dir_path = QDir::homePath() + "/" + tr("Pictures");
    // browse the directory
    QDirIterator it(QDir(dir_path), QDirIterator::NoIteratorFlags);
    while(it.hasNext())
    {
        // check if the file is a valic picture
        QPixmap pix;
        QString file = it.next();
        if(pix.load(file))
        {
            // add it to the view
            QListWidgetItem *item = new QListWidgetItem(m_list);
            item->setIcon(QIcon(file));
            item->setText("");
            m_list_hash.insert(item, file);
            m_list->addItem(item);

            // if the user specified no image, use the first we find
            if(file_name.isEmpty())
                file_name = QFileInfo(file).fileName();
        }
    }

    // a click on a picture displays it
    connect(m_list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(m_image_clicked(QListWidgetItem*)));

    // update the text of the label
    m_dir_label->setText(dir_path);

    // load the picture
    m_viewer->setPicture(dir_path + "/" + file_name);

    // set the title of the window, and an icon
    setWindowTitle(file_name);
    setWindowIcon(QIcon(dir_path + "/" + file_name));

    // this prevents for some display bugs
    setMinimumSize(200,200);
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    // resize widgets

    // the list should be always 110 pixels high and adjust to the window in width
    m_list->resize(width(), 110);

    // the bar full the width of the window and keeps its defautl height
    m_toolbar->resize(width(), m_toolbar->height());

    // the view fill the rest
    int view_height = height() - m_toolbar->height() - m_list->height();
    m_viewer->resize(width(), view_height);

    // replace the widgets vertically
    m_viewer->move(0,0);
    m_toolbar->move(0, m_viewer->height());
    m_list->move(0, m_viewer->height() + m_toolbar->height());
}

void MainWindow::m_image_clicked(QListWidgetItem *it)
{
    // just load the picture
    QString file = m_list_hash.value(it);
    m_viewer->setPicture(file);

    // set the title of the window, and an icon
    setWindowTitle(QFileInfo(file).fileName());
    setWindowIcon(QIcon(file));
}

