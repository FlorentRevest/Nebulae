/*
     * You should have received a copy of the GNU General Public License
     * along with Nebulae. If not, see <http://www.gnu.org/licenses/>.
     */

#include "mainwindow.h"
#include "searchlineedit.h"

#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QSettings>
#include <QtCore/QDirIterator>

#include <QtGui/QLabel>
#include <QtGui/QAction>
#include <QtGui/QToolBar>
#include <QtGui/QListWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QX11EmbedContainer>

#include <QtDebug>

#include "nebulae-desktopentry.h"

void MainWindow::m_init_display()
{
    // display is made of a ListWidget
    m_display = new QListWidget(this);
    setCentralWidget(m_display);

    // give it a better look
    m_display->setViewMode(QListWidget::IconMode);          // use icons instead of list
    m_display->setIconSize(QSize(64,64));
    m_display->setResizeMode(QListWidget::Adjust);
    m_display->setMovement(QListWidget::Static);
    m_display->setGridSize(QSize(128, 128));
    m_display->setWordWrap(true);

    connect(m_display, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(m_item_clicked(QListWidgetItem*)));
}

void MainWindow::m_populate_toolbars()
{
    // the top toolbar, with a title and a search bar
    QToolBar *top_bar = new QToolBar(this);
    QHBoxLayout *l = new QHBoxLayout(top_bar);
    l->setContentsMargins(0,0,0,0);
    m_title_label = new QLabel("<strong>" + tr("Administration Panel") + "</strong>", this);
    m_title_label->setIndent(10);
    m_search_bar = new QLineEdit(this);
    m_search_bar->setPlaceholderText(tr("Find..."));
    l->addWidget(m_title_label);
    l->addStretch();
    l->addWidget(m_search_bar);
    QWidget *w = new QWidget(this);
    w->setLayout(l);
    top_bar->addWidget(w);
    top_bar->setAllowedAreas(Qt::TopToolBarArea);
    top_bar->setMovable(false);
    top_bar->setFloatable(false);
    addToolBar(top_bar);

    connect(m_search_bar, SIGNAL(textEdited(QString)), this, SLOT(m_search(QString)));

    // the sidebar with the cat list
    QToolBar *side_bar = new QToolBar(this);

    // add an action to return to the home page
    m_home_act = side_bar->addAction(QIcon::fromTheme("go-home"), tr("Home"), this, SLOT(m_home_clicked()));

    foreach(Category *cat, m_cats) {
        QAction *cat_act = side_bar->addAction(cat->icon(), cat->title(), this, SLOT(m_cat_clicked()));
         cat_act->setProperty("Category", cat->title());      // remember its category with its title
    }
    side_bar->setIconSize(QSize(48,48));
    side_bar->setAllowedAreas(Qt::LeftToolBarArea);
    side_bar->setMovable(false);
    side_bar->setFloatable(false);
    addToolBar(Qt::LeftToolBarArea, side_bar);
}

void MainWindow::m_populate_categories()
{
    m_cats.clear();

    // create empty categories
    Category *cat_app = new Category(tr("Appearance"), QStringList() << "DesktopSettings", QIcon::fromTheme("preferences-desktop-theme"));
    m_cats.append(cat_app);

    Category *cat_net = new Category(tr("Internet and networking"), QStringList() << "NetworkSettings", QIcon::fromTheme("applications-internet"));
    m_cats.append(cat_net);

    Category *cat_hard = new Category(tr("Hardware"), QStringList() << "HardwareSettings", QIcon::fromTheme("applications-electronics"));
    m_cats.append(cat_hard);

    Category *cat_sys = new Category(tr("System"), QStringList() << "System", QIcon::fromTheme("applications-system"));
    m_cats.append(cat_sys);

    Category *cat_secu = new Category(tr("Security"), QStringList() << "Security", QIcon::fromTheme("stock_keyring"));
    m_cats.append(cat_secu);

    Category *cat_others = new Category(tr("Others"), QStringList() << "Settings", QIcon::fromTheme("applications-other"));
    m_cats.append(cat_others);

    m_virtual_search_cat = new Category(tr("Search results..."), QStringList(), QIcon());

    // explore fdesktop standard applications directory
    QStringList already_added; // used to avoid duplicates;
    QDirIterator i("/usr/share/applications/", QStringList() << "*.desktop", QDir::AllEntries, QDirIterator::Subdirectories);
    while(i.hasNext())
    {
        DesktopEntry d(i.filePath());
        QStringList cats = d.categories();
        foreach(Category *cat, m_cats)
        {
            // okay, look if it corresponds to any of our categories
            foreach(QString _category, cat->fdesktop_cat())
            {
                // also check it has not been added in an y other category yet
                if(cats.contains(_category) && cats.contains("Settings") && !already_added.contains(i.filePath()))
                {
                    // it corresponds ! add it  to the concerned category
                    cat->addEntry(d);
                    already_added.append(i.filePath()); // items shouldn't be added in two categories
                    break;
                } // cats.contains
            } // foreach
        } // foreach

        i.next();
    } // i.hasNext()
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // initialize the first categories
    m_populate_categories();

    // build the main elements of the GUI //
    m_init_display();
    m_populate_toolbars();

    // display home buy default
    m_home_clicked();

    qDebug() << tr("Test phrase");
}

void MainWindow::m_cat_clicked()
{
    // update the view with the icons of the currently clicked //

    // wich category has been selected ?
    QAction *act = qobject_cast<QAction *>(sender());       // get back the clicked action
    if(!act)
        return;
    Category *cat;
    QString title = act->property("Category").toString();
    foreach(Category *_cat, m_cats)
    {
        if(_cat->title() == title)
        {
            cat = _cat;
            break;
        }
    }
    if(!cat) return;

    // display every elements
    m_display_category(cat);
}

void MainWindow::m_display_category(Category *cat)
{
    // remove old actions
    m_display->clear();
    m_exec_values.clear();
    m_at_home = false;

    QList<DesktopEntry> entries = cat->entries();
    foreach(DesktopEntry d, entries)
    {
        // only display entries of type Application
        if(d.type() != DesktopEntry::Application)
            continue;

        // check we can display it
        if(!d.onlyShowIn().isEmpty() && !d.onlyShowIn().contains("X-NEBULAE")) {
            continue;
        }
        if(!d.notShowIn().isEmpty() && d.notShowIn().contains("X-NEBULAE")){
            continue;
        }

        QString name = d.name(QLocale::system().name()).isEmpty() ? d.name() : d.name(QLocale::system().name());

        // add it in the view with the correct informations
        QListWidgetItem *item = new QListWidgetItem(m_display);
        item->setIcon(d.icon());
        item->setText(name);
        m_display->addItem(item);

        // remember it's exec value
        m_exec_values.insert(item, d.exec());

        qDebug() << "item added...";
    } // foreach entry

    // update the title of the window
    setWindowTitle(cat->title() + " - " + tr("Nebulae Administration Panel"));
    m_title_label->setText(" <strong>" + cat->title() + "</strong>");
}

void MainWindow::m_item_clicked(QListWidgetItem *it)
{
    // the user clicked on an item //

    if(m_at_home)
    {
        qDebug() << "click on " << it->text();
        // the item corresponds to a category
        foreach(Category *cat, m_cats)
        {
            qDebug() << "is " << cat->title() << it->text() << "??";
            if(cat->title() == it->text())
            {
                qDebug() << "yes";
                // it's the category clicked
                m_display_category(cat);
               break;
            }
        }
        return;
    }

    // get back the exec value of the desktop entry
    QString exec = m_exec_values.value(it);
    if(!exec.isEmpty())
    {
        // launch it
        exec.remove("%u", Qt::CaseInsensitive);
        QProcess p;
        p.startDetached(exec);
    }
}

void MainWindow::m_home_clicked()
{
    m_display->clear();
    m_exec_values.clear();

    // display a button for each category
    foreach(Category *cat, m_cats)
    {
        QListWidgetItem *item = new QListWidgetItem(m_display);
        item->setIcon(cat->icon());
        item->setText(cat->title());
        m_display->addItem(item);

        // the m_exec_values hash is now used to store categories
        m_exec_values.insert(item, cat->title());
    }

    setWindowTitle(tr("Nebulae Administration Panel"));
    m_title_label->setText(" <strong>" + tr("Home") + "</strong>");
    m_at_home = true;
}

void MainWindow::m_search(QString text)
{
    if(text.isEmpty()) {
        m_home_clicked();
        return;
    }

    // reset previous searches
    m_virtual_search_cat->clearEntries();

    // display every entry that contains text it it's name
    foreach(Category *cat, m_cats)
    {
        // explore each action of each cat
        foreach(DesktopEntry d, cat->entries())
        {
            QString name = d.name(QLocale::system().name()).isEmpty() ? d.name() : d.name(QLocale::system().name());
            QString fname = d.filePath().split('/').last();

            // look if the search corresponds wether to its file name or to it's localized name
            if(fname.contains(text, Qt::CaseInsensitive) || name.contains(text, Qt::CaseInsensitive))
                m_virtual_search_cat->addEntry(d);
        }
    }

    m_display_category(m_virtual_search_cat);
}

MainWindow::~MainWindow()
{
    foreach(Category *cat, m_cats)
    {
        delete cat;
    }
}
