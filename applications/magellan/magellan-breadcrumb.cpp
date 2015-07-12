#include "magellan-breadcrumb.h"

#include <QtCore/QVariant>

#include <QtGui/QMenu>
#include <QtGui/QAction>

#include <Kommodity/GIO/File>
#include <Kommodity/GIO/FileInfo>
#include <Kommodity/GIO/ThemedIcon>
using namespace Kommodity::GIO;

#include <QtDebug>

// used for directory listing
// sort a list of file infos alphabetically
inline QList<FileInfo> sort_info_list(QList<FileInfo> input)
{
    QList<FileInfo> output;

    // sort alphabetically
    while(!input.isEmpty())
    {
        FileInfo higher = input.first();
        int index = 0;
        for(int i = 0; i < input.count(); i++)
        {
            FileInfo inf = input.at(i);
            if(QString::localeAwareCompare(higher.getName(),inf.getName()) > 0) {
                higher = inf;
                index = i;
            }
        }
        output.append(higher);
        input.removeAt(index);
    }

    return output;
}

// list contents of a directory as a file info list
bool directory_list(QUrl url, QList<FileInfo> *info)
{
    QList<FileInfo> dirs;

    // begin listing
    File file(url);
    if(file.isNull())
        return false;

    Error error;
    FileEnumerator fileEnumerator = file.enumerateChildren("*", File::QueryInfoNorm, error);

    if(error.hasError())
        return false;

    // split between files and dirs
    else
    {
        for (;;)
        {
            FileInfo fileInfo = fileEnumerator.nextFile(error);

            if(error.hasError())
                return false;

            if (fileInfo.isNull())
                break;  // finished

            if(fileInfo.getName().startsWith('.'))
                continue;

            if(fileInfo.getContentType() == "inode/directory")
                dirs.append(fileInfo);
        }

        fileEnumerator.close(error);
        if(error.hasError())
            return false;
    }

    // sort both lists
   info->append(sort_info_list(dirs));
   return true;
}

QIcon file_info_icon(FileInfo inf)
{
    Icon *in = inf.getIcon();

    // try theme icon
    ThemedIcon *ic = dynamic_cast<ThemedIcon*>(in);
    foreach(QString name, ic->getNames())
    {
        QIcon ret = QIcon::fromTheme(name);
        if(!ret.isNull())
            return ret;
    }

    // not found... return blank file
    return QIcon::fromTheme("inode-directory");
}

///////////////////////////////////////////////////////////////////

Breadcrumb::Breadcrumb(QWidget *parent) :
    QWidget(parent)
{
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0,0,0,0);
    setLayout(m_layout);
}

void Breadcrumb::setUrl(QUrl url)
{
    qDebug() << url << " received [breadcrumb]";

    // clear
    foreach(QToolButton *btn, m_btns)
    {
        m_btns.removeOne(btn);
        m_layout->removeWidget(btn);
        delete btn;
    }

    QString path = "/";
    QStringList prts = url.path().split('/');
    prts.removeAll("");

    QToolButton *root_btn = new QToolButton(this);

    if(url.scheme() == "file")
    {
        QString username = getenv("USER");
        if(prts.count() > 1)
        {
            if(prts.first() == "home" && prts.at(1) == username)
            {
                path = "/home/" + username + "/";
                root_btn->setText(username);
                prts.removeFirst();
                prts.removeFirst();
            }

        // else if(prts.first() == "/media")
            else
                root_btn->setText("/");

        }
        else
            root_btn->setText("/");
    }
    else
        root_btn->setText(url.scheme() + tr(" as ") + url.userName() + " on " + url.host());

    QUrl root_btn_url = url;
    root_btn_url.setPath(path);
    root_btn->setProperty("Url", QVariant(root_btn_url));
    m_layout->addWidget(root_btn);
    m_btns.append(root_btn);

    // create a menu button with subdirectories
    QList<FileInfo> infs;
    if(directory_list(root_btn_url, &infs) && !(infs.isEmpty()))
    {
        QMenu *mnu = new QMenu(this);
        foreach(FileInfo inf, infs)
        {
            QAction *act = mnu->addAction(file_info_icon(inf), QString::fromUtf8(inf.getDisplayName().toAscii()), this, SLOT(m_act_clicked()));
            act->setProperty("Url", root_btn_url.toString() + (root_btn_url.path().endsWith("/") ? QString() : "/") + QString::fromUtf8(inf.getDisplayName().toAscii()));
        }
        root_btn->setMenu(mnu);
        root_btn->setPopupMode(QToolButton::MenuButtonPopup);
    }

    connect(root_btn, SIGNAL(clicked()), this, SLOT(m_btn_clicked()));

    foreach(QString part, prts)
    {
        path.append(part + "/");
        QToolButton *btn = new QToolButton(this);
        btn->setText(part);
        QUrl btn_url = url;
        btn_url.setPath(path);
        btn->setProperty("Url", btn_url);
        m_layout->addWidget(btn);
        m_btns.append(btn);

        // create a menu button with subdirectories
        QList<FileInfo> infs;
        if(directory_list(btn_url, &infs) && !(infs.isEmpty()))
        {
            QMenu *mnu = new QMenu(this);
            foreach(FileInfo inf, infs)
            {
                QAction *act = mnu->addAction(file_info_icon(inf), QString::fromUtf8(inf.getDisplayName().toAscii()), this, SLOT(m_act_clicked()));
                act->setProperty("Url", btn_url.toString() + (btn_url.path().endsWith("/") ? QString() : "/") + QString::fromUtf8(inf.getDisplayName().toAscii()));
            }
            btn->setMenu(mnu);
            btn->setPopupMode(QToolButton::MenuButtonPopup);
        }

        connect(btn, SIGNAL(clicked()), this, SLOT(m_btn_clicked()));
    }

     qDebug() << url << " loading finished [breadcrumb] \n <<<<<<<<<<<<<<<<<<<<";
}

void Breadcrumb::m_btn_clicked()
{
    QToolButton *btn = qobject_cast<QToolButton *>(sender());
    if(btn) {
        qDebug() << btn->property("Url").toUrl() << " emitted [breadcrumb]";
        emit urlChanged(btn->property("Url").toUrl());
    }
}

void Breadcrumb::m_act_clicked()
{
    QAction *act = qobject_cast<QAction*>(sender());
    if(act) {
        qDebug() << act->property("Url").toUrl() << " emitted [breadcrumb]";
        emit urlChanged(act->property("Url").toUrl());
    }
}

