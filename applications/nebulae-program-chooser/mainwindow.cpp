#include "mainwindow.h"
#include "nebulae-desktopentry.h"
#include "programdefaultseditor.h"

#include <QtCore/QSettings>
#include <QtCore/QDirIterator>

#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QDockWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QCommandLinkButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // build the entry list
    QDirIterator it("/usr/share/applications", QDirIterator::Subdirectories);
    for(it; it.hasNext(); it.next())
        m_entries.append(new DesktopEntry(it.filePath()));

    // create a side view that display every application
    m_side_doc = new QDockWidget(tr("Applications"), this);
    m_side_doc->setAllowedAreas(Qt::LeftDockWidgetArea);
    m_side_doc->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, m_side_doc);

    m_list = new QListWidget(this);
    m_list->setViewMode(QListWidget::ListMode);
    m_side_doc->setWidget(m_list);

    // populate it
    foreach(DesktopEntry *d, m_entries)
    {
        QString name = d->name();

        // check this entry is interesting for our program
        if(!name.isEmpty() && !d->mimeTypes().isEmpty())
        {
            QListWidgetItem *it = new QListWidgetItem(m_list);
            it->setText(name);
            it->setIcon(d->icon());
            m_list->addItem(it);
            m_entries_items.insert(it, d);
        }
    }

    // create a container for the others widgets
    m_side_widget = new QWidget(this);
    QVBoxLayout *side_layout = new QVBoxLayout(m_side_widget);
    m_side_widget->setLayout(side_layout);
    setCentralWidget(m_side_widget);

    // initialize those widgets
    QWidget *lbl_widget = new QWidget(this);
    QHBoxLayout *lbl_layout = new QHBoxLayout(lbl_widget);
    lbl_layout->setContentsMargins(0,0,0,0);
    m_ic_label = new QLabel(this);
    m_ic_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_name_label = new QLabel(tr("Name"), this);
    m_name_label->setWordWrap(true);
    m_defaults_label = new QLabel(tr("Defaults"), this);
    m_defaults_label->setWordWrap(true);
    lbl_layout->addWidget(m_ic_label, Qt::AlignLeft);
    lbl_layout->addWidget(m_name_label, Qt::AlignLeft);
    lbl_widget->setLayout(lbl_layout);
    side_layout->addWidget(lbl_widget);
    side_layout->addStretch();
    side_layout->addWidget(m_defaults_label);

    // the bottom buttons
    QCommandLinkButton *btn  = new QCommandLinkButton(tr("Set this program as the default program"), tr("Use the selected program to opened all the types of files it can open by default"), this);
    QCommandLinkButton *btn2 = new QCommandLinkButton(tr("Change default parameters for this program"), tr("Choose the types of files that the selected program can open by default"), this);
    connect(btn, SIGNAL(clicked()), this, SLOT(m_set_default()));
    connect(btn2, SIGNAL(clicked()), this, SLOT(m_edit_program()));
    side_layout->addWidget(btn);
    side_layout->addWidget(btn2);

    // the settings
    m_global = new QSettings("/usr/share/applications/defaults.list", QSettings::NativeFormat, this);
    m_local = new QSettings(QDir::homePath() + "/.local/share/applications/defaults.list", QSettings::NativeFormat, this);
    m_global->beginGroup(QString("Default Applications").toUtf8());
    m_local->beginGroup(QString("Default Applications").toUtf8());

    connect(m_list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(m_list_item_clicked(QListWidgetItem*)));
}

void MainWindow::m_set_default()
{
    // the selected program now opens all the types it manages
    DesktopEntry *app = m_entries_items.value(m_current_item);
    foreach(QString type, app->mimeTypes())
        m_local->setValue(type, app->filePath().split('/').last());

    // update the view //

    // compute how many types this program can open :
    int defaults = app->mimeTypes().count();

    // compute how many types it actually opens
    int opened = 0;
    foreach(QString def, app->mimeTypes())
    {
        if(m_local->value(def).toString() == app->filePath().split('/').last())
            opened++;
        else if(m_global->value(def).toString() == app->filePath().split('/').last())
            opened++;
    }
    m_defaults_label->setText(QString::number(opened) + " defaults out of " + QString::number(defaults));
}

void MainWindow::m_edit_program()
{
    QListWidgetItem *it = m_list->selectedItems().first(); // a single item is selected
    ProgramDefaultsEditor *ed = new ProgramDefaultsEditor(m_entries_items.value(it), this);
    setCentralWidget(ed);
    m_side_doc->hide();
    connect(ed, SIGNAL(finished()), this, SLOT(m_editor_finished()));
}

void MainWindow::m_editor_finished()
{
    ProgramDefaultsEditor *ed = qobject_cast<ProgramDefaultsEditor*>(centralWidget());
    setCentralWidget(new QWidget());
    m_side_widget->hide();
    delete ed;
    m_side_doc->show();
}

void MainWindow::m_list_item_clicked(QListWidgetItem *it)
{
    // the user clicked on an app in the list //

    // get back the clicked app
    DesktopEntry *app = m_entries_items.value(it);
    m_ic_label->setPixmap(app->icon().pixmap(48,48));
    m_name_label->setText(app->name() + '\n' + app->genericName());

    // compute how many types this program can open :
    int defaults = app->mimeTypes().count();

    // compute how many types it actually opens
    int opened = 0;
    foreach(QString def, app->mimeTypes())
    {
        if(m_local->value(def).toString() == app->filePath().split('/').last())
            opened++;
        else if(m_global->value(def).toString() == app->filePath().split('/').last())
            opened++;
    }

    m_defaults_label->setText(QString::number(opened) + " defaults out of " + QString::number(defaults));

    m_current_item = it;
}

MainWindow::~MainWindow()
{
    foreach(DesktopEntry *d, m_entries)
        delete d;
}
