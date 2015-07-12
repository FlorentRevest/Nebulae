#include "mainwindow.h"
#include "desktopentry.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QSettings>

#include <QtGui/QIcon>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

#include <QtDebug>

QStringList xdg_config_dirs;
QStringList xdg_data_dirs;
QStringList default_app_dirs;
QStringList default_merge_dirs;
QStringList default_directory_dirs;
QStringList default_menu_dirs;

// this implementation dose not manages yet moved menus and legacy dirs

struct Menu {
    // default name
    QString name;

    // file containing metainfo
    QString directory;
    QStringList directoryDirs;

    // dirs form which applications must be loaded
    QStringList appDirs;

    // should this menu use allocated entries ?
    bool useAllocated;

    // include/exclude rules fore entries
    QString excludeRules;
    QString includeRules;

    // internal objet for display the menu
    QMenu *qmenu;

    // childs submenus and parent menu
    QList<Menu> subMenus;
};

QString createMatchRule(QDomElement rule)
{
    return QString();
}

bool matchRule(QString rule, QString text)
{

}


QDomNode cleanMenu(QDomNode menu, QDomDocument *doc)
{
    QHash<QString, QDomNode> menus;
    QHash<QString, QDomNode> appdirs;
    QHash<QString, QDomNode> dirdirs;
    QHash<QString, QDomNode> dirs;

    QDomNode child = menu.firstChildElement();
    while(!child.isNull())
    {
        QString tn = child.toElement().tagName();

        // clean useless elements
        if(tn == "MergeFile" || tn == "MergeDir" || tn == "DefaultMergeDirs" || tn == "LegacyDir" || child.isComment())
        {
            QDomNode newChild = child.nextSibling();
            menu.removeChild(child);
            child = newChild;
            continue;
        }

        // expand default app dirs and default directory dirs
        else if(tn == "DefaultAppDirs")
        {
            foreach(QString dir, default_app_dirs)
            {
                // earlier dirs go later so they have priority
                QDomElement el = doc->createElement("AppDir");
                menu.insertAfter(el, child);

                QDomText n = doc->createTextNode(dir);
                el.appendChild(n);
            }
            QDomNode newChild = child.nextSibling();
            menu.removeChild(child);
            child = newChild;
            continue;
        }

        // and consolidate single elements
        else if(tn == "AppDirs")
        {
            QString dir = child.toElement().text();
            if(appdirs.keys().contains(dir))
            {
                // later take precedence, remove the older
                menu.removeChild(appdirs.value(dir));
                appdirs.remove(dir);
            }
            appdirs.insert(dir, child);
        }

        // idem for directory dirs
        else if(tn == "DefaultDirectoryDirs")
        {
            foreach(QString dir, default_directory_dirs)
            {
                // earlier dirs go later so they have priority
                QDomElement el = doc->createElement("DirectoryDir");
                menu.insertAfter(el, child);

                QDomText n = doc->createTextNode(dir);
                el.appendChild(n);
            }
            QDomNode newChild = child.nextSibling();
            menu.removeChild(child);
            child = newChild;
            continue;
        }

        else if(tn == "DirectoryDir")
        {
            QString dir = child.toElement().text();
            if(dirdirs.keys().contains(dir))
            {
                // later take precedence, remove the older
                menu.removeChild(dirdirs.value(dir));
                dirdirs.remove(dir);
            }
            dirdirs.insert(dir, child);
        }

        else if(tn == "Directory")
        {
            QString file = child.toElement().text();
            if(dirs.keys().contains(file))
            {
                // later take precedence, remove the older
                menu.removeChild(dirs.value(file));
                dirs.remove(file);
            }
            dirs.insert(file, child);
        }

        // consolidate menus with the same names
        else if(tn == "Menu")
        {
            QString name = child.firstChildElement("Name").text();
            if(menus.keys().contains(name))
            {
                QDomNode old = menus.value(name);
                QDomNode subChild = old.firstChild();
                while(!subChild.isNull())
                {
                    if(subChild.toElement().tagName() != "Name")
                        child.insertBefore(subChild, child.firstChild());
                    old.removeChild(subChild);
                    subChild = old.firstChild();
                }
                menu.removeChild(old);
                menus.remove(name);
                menus.insert(name, child);
            }
            else
                menus.insert(name, child);
        }
        child = child.nextSibling();
    }

    // clean submenus
    foreach(QString key, menus.keys())
    {
        QDomNode mn = menus.value(key);
        QDomNode cleaned = cleanMenu(mn, doc);
        menu.replaceChild(cleaned, mn);
    }

    // TODO: implement move operations
    return menu;
}

QStringList mergedFiles;
void mergeElement(QDomNode *node, QString filePath = QString(), bool deleteNode = true)
{
    // in some case (MergeFile nodes) path to merge is directly contained in the node's text
    filePath = filePath.isEmpty() ? node->toElement().text() : filePath;

    // ensure this file is not already included : it might be a loop
    if(mergedFiles.contains(filePath))
        return;

    // replace node by the first element of the file on which it points to
    QDomNode parent = node->parentNode();

    // opens the given file and begin merging all files
    QFile f(filePath);

    qDebug() << "OPENING " << filePath << "...";
    if(f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // avoid recursives infinite loops : this file won't be merged again (WE WON'T GET MERGED AGAAAAIN !)
        mergedFiles.append(filePath);

        // open the document, and read it's first child, which should be <Menu>
        QDomDocument newDoc;
        newDoc.setContent(&f);
        QDomNode newNode = newDoc.firstChild();
        if(!newNode.isNull() && newNode.toElement().tagName() == "Menu")
        {
            // Move all childs in the origin document
            QDomNode subNode = newNode.firstChild();
            while(!subNode.isNull())
            {
                QDomElement subEl = subNode.toElement();

                // this document is only virtual : it's useless to write comments. The spec also tells to remove names
                if(subNode.isComment() || subEl.tagName() == "Name")
                {
                    newNode.removeChild(subNode);
                    subNode = newNode.firstChild();
                    continue;
                }

                // Use recursivity
                if(subEl.tagName() == "MergeFile" && !mergedFiles.contains(subEl.text()))
                    mergeElement(&subNode);

                // merge all the files in te directory
                else if(subEl.tagName() == "MergeDir")
                {
                    QDir d(subEl.text());
                    if(d.exists())
                    {
                        QStringList toMerge = QDir(subEl.text()).entryList(QStringList() << "*.menu", QDir::Files);
                        foreach(QString _f, toMerge)
                            mergeElement(&subNode, subEl.text() + "/" + _f, false);
                    }
                }

                // merge all the files in all the defaults merge directories (configdir/menus/applications-merged)
                else if(subEl.tagName() == "DefaultMergeDirs")
                {
                    foreach(QString dir, default_merge_dirs)
                    {
                        QDir d(dir);
                        if(d.exists())
                        {
                            QStringList toMerge = d.entryList(QStringList() << "*.menu", QDir::Files);
                            foreach(QString _f, toMerge)
                                mergeElement(&subNode, dir + "/" + _f, false);
                        }
                    }
                }

                // once the node treated (eventually merged) insert it
                parent.insertBefore(subNode, *node);

                // remove it from the oprevious document and take another one
                newNode.removeChild(subNode);
                subNode = newNode.firstChild();
            }

            // delete the node that initiated the merge (optionnal because of DefaultMergeDirs & cie)
            if(deleteNode)
                parent.removeChild(*node);
        }
        qDebug() << "SUCCESS";
    }
    else
        qDebug() << "ERROR";
}

QDomDocument *mergeAll(QString file)
{
    // create a virtual document that will be used to build the complete tree
    QDomDocument *doc = new QDomDocument();

    // opens the given file and begin merging all files
    QFile f(file);
    qDebug() << "OPENING " << file;
    if(f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // fill our virtual doc with the contents of the file
        doc->setContent(&f);

        // and then search for mergings
        QDomNode n = doc->firstChild().firstChild();
        while(!n.isNull())
        {
            if(n.isComment())
            {
                n = n.nextSibling();
                continue;
            }

            QDomElement el = n.toElement();
            if(el.tagName() == "MergeFile")
                mergeElement(&n);

            // for the merge dirs and default merge dirs items, see the function above
            else if(el.tagName() == "MergeDir")
            {
                QDir d(el.text());
                if(d.exists())
                {
                    QStringList toMerge = QDir(el.text()).entryList(QStringList() << "*.menu", QDir::Files);
                    foreach(QString _f, toMerge)
                        mergeElement(&n, el.text() + "/" + _f, false);
                }
            }

            else if(el.tagName() == "DefaultMergeDirs")
            {
                foreach(QString dir, default_merge_dirs)
                {
                    QDir d(dir);
                    if(d.exists())
                    {
                        QStringList toMerge = d.entryList(QStringList() << "*.menu", QDir::Files);
                        foreach(QString _f, toMerge)
                            mergeElement(&n, dir + "/" + _f, false);
                    }
                }
            }

            // continue with the next item
            n = n.nextSibling();
        }

        doc->replaceChild(cleanMenu(doc->firstChild(), doc), doc->firstChild());
        // we finished exploring our document
        qDebug() << "SUCCESS";
    }
    else
        qDebug() << "ERROR";


    qDebug() << doc->toString();
    return doc;
}

Menu parseMenu(QDomElement e, Menu parent = Menu())
{
    Menu menu;
    // menu.parentMenu = parent;
    menu.qmenu = new QMenu();

    // fill default values
    menu.useAllocated = false;
    if(!parent.name.isEmpty())
    {
        menu.directoryDirs.append(parent.directoryDirs);
        menu.appDirs.append(parent.appDirs);
    }

    for(QDomElement el = e.firstChildElement(); !el.isNull(); el = el.nextSiblingElement())
    {
        QString tn = el.tagName();

        // fill datas

        if(tn == "Name")
            menu.name = el.text();
        else if(tn == "Directory")
            menu.directory = el.text();
        else if(tn == "DirectoryDir")
            menu.directoryDirs.append(el.text());
        else if(tn == "AppDir")
            menu.appDirs.append(el.text());
        else if(tn == "OnlyUnallocated")
            menu.useAllocated = false;
        else if(tn == "NotOnlyUnallocated")
            menu.useAllocated = true;
        else if(tn == "Include")
            menu.includeRules = createMatchRule(el);
        else if(tn == "Exclude")
            menu.excludeRules = createMatchRule(el);
        else if(tn == "Menu")
            menu.subMenus.append(parseMenu(el, menu));
    }

    return menu;
}

void displayMenu(Menu menu, Menu parent = Menu())
{
    // search the metadata file
    QString dirFile;
    foreach(QString dir, menu.directoryDirs)
    {
        QString pth = dir + "/" + menu.directory;
        if(QFile(pth).exists() && !pth.isEmpty())
            dirFile = pth;
        // don't break : lasts take precedence
    }

    // then read metas
    DesktopEntry d(dirFile);
    menu.qmenu->setTitle(d.name());
    menu.qmenu->setIcon(QIcon::fromTheme(d.icon()));

    // set default title if failed
    if(menu.qmenu->title().isEmpty())
        menu.qmenu->setTitle(menu.name);

    if(!parent.name.isEmpty())
    {
        // our menu is empty, simply add it
        if(parent.qmenu->isEmpty())
            parent.qmenu->addMenu(menu.qmenu);

        // add it in the right place, alphabetically
        bool added = false;
        foreach(QAction *m, parent.qmenu->actions())
        {
            // it's lower, insert it before
            if(menu.name < m->text())
            {
                parent.qmenu->insertMenu(m, menu.qmenu);
                added = true;
                break;
            }
        }

        // not placed ? place it at the end
        if(!added)
            parent.qmenu->addMenu(menu.qmenu);
    }

    foreach(Menu m, menu.subMenus)
        displayMenu(m, menu);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // get default values
    xdg_config_dirs = QString(getenv("XDG_CONFIG_DIRS")).split(':');
    if(xdg_config_dirs.isEmpty())
        xdg_config_dirs.append("/etc/xdg");

    QString xdg_config_home = getenv("XDG_CONFIG_HOME");
    if(xdg_config_home.isEmpty())
        xdg_config_home = QDir::homePath() + "/.config";
    xdg_config_dirs.prepend(xdg_config_home);

    xdg_data_dirs = QString(getenv("XDG_DATA_DIRS")).split(':');
    if(xdg_data_dirs.isEmpty())
        xdg_data_dirs << "/usr/local/share" << "usr/share";

    QString xdg_data_home = getenv("XDG_DATA_HOME");
    if(xdg_data_home.isEmpty())
        xdg_data_home = QDir::homePath() + "/.local/share";
    xdg_data_dirs.prepend(xdg_data_home);

    // initialize paths : firsts take precedence
    foreach(QString dir, xdg_data_dirs) {
        default_app_dirs.append(dir + "/applications");
        default_directory_dirs.append(dir + "/desktop-directories");
    }
    foreach(QString dir, xdg_config_dirs) {
        default_merge_dirs.append(dir + "/menus/applications-merged");
        default_menu_dirs.append(dir + "/menus");
    }

    // search the default applications.menu file :
    QDomDocument *virtualMenu;
    foreach(QString dir, default_menu_dirs) {
        // the first we find have precedence
        if(QFile(dir + "/applications.menu").exists()) {
            virtualMenu = mergeAll(dir + "/applications.menu");
            break;
        }
    }

    // now, build our menu tree
    Menu root = parseMenu(virtualMenu->firstChildElement());
    displayMenu(root);

    QMenuBar *mnb = new QMenuBar(this);
    setMenuBar(mnb);
    mnb->addMenu(root.qmenu);
}

MainWindow::~MainWindow()
{

}
