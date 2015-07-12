#include "openasdialog.h"

#include <QtCore/QProcess>
#include <QtCore/QLocale>
#include <QtCore/QSettings>
#include <QtCore/QDirIterator>

#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>

#include "listwidgetitem.h"

OpenAsDialog::OpenAsDialog(const QString &filePath, const QString &type, QWidget *parent) : QDialog(parent), m_file_path(filePath)
{
    // make the window a bit more presentable :)
    setWindowTitle(tr("Open with..."));
    resize(230, 320);
    QSize s = QApplication::desktop()->size();      // gets the size of the desktop widget, e.g. the screen size :)
    move(s.width() / 2 - width() / 2, s.height() / 2 - height() / 2); // replace our widget at the middle of the screen, centered
    QGridLayout *l = new QGridLayout(this);
    setLayout(l);

    // We will display the items in a QListView
    m_list = new QListWidget(this);
    m_list->setViewMode(QListWidget::ListMode);
    m_list->setIconSize(QSize(24,24));
    l->addWidget(m_list);

    // if the  mimetype is incorrect...
    // TODO: there"s probably a more intelligent way to do this... some file doesn't have mimetypes and are not plain text :/
    QString mimetype = type.isEmpty() ? "text/plain" : type;

    // Ok, what we're goin to do is browse the folder where all the launchers, of all the applications, are put, /usr/share/applications
    // for each laucher (.desktop) we meet, we read it to see if he says he cans open our mimetype, passed as parameter

    // list all the .desktop present in the applications folder, including subdirectories :
    QDirIterator it("/usr/share/applications", QStringList("*.desktop"), QDir::Files, QDirIterator::Subdirectories);

    while(it.hasNext())
    {
        // the .desktop specification separates the elements of a list with ";". If we use QSettings, we will get only the first element,
        // that's why we will have to read directrly the file with a QFile
        QString file = it.next();
        QFile f(file);
        if(f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            // the file is opened, browse it :
            while (!f.atEnd())
            {
                // we read one line of the file
                QByteArray line = f.readLine();

                if(line.startsWith("MimeType="))    
                {
                    // this line specify the MimeTypes that this application can open
                    line = line.mid(9); // remove the "MimeType=" at the beginning so that we get only the categories

                    // the .desktop specification separates elements by a ;
                    QString sline = line; // convert into a QString
                    QStringList mimetypes = sline.split(';');

                    // check that the mimetype we have to open is managed by this app...
                    if(mimetypes.contains(mimetype))
                    {
                        // it works ! :)
                        m_add_application(file);
                    }
                }
            }   
            // we reached the end of the file
        }   
        // don't notify the user if the .desktop couldn't be open, they are so many, that doesn't matter
    }
    // we reached the end of the directory

    // finally add an ok button !
    QPushButton *ok_btn = new QPushButton(tr("Ok"));
    connect(ok_btn, SIGNAL(clicked()), this, SLOT(m_launch()));
    l->addWidget(ok_btn);
}

void OpenAsDialog::m_add_application(const QString &application_desktop_file_path)
{
    // we said above that QSettings couldn't work for a string list, but it's a bit
    // better (faster) than QFile for single values, we will use it here
    QSettings s(application_desktop_file_path, QSettings::NativeFormat);
    s.beginGroup("Desktop Entry");

    // first of all get the name and the icon of the application
    QIcon ic = QIcon::fromTheme(s.value("Icon").toString());
    QString name = s.value("Name[" + QLocale::system().name().section('_',0,0) + "]").toString();

    // if we dont have a name with the locale, fallback to the default one
    name = name.isEmpty() ? s.value("Name").toString() : name;

    // we will use a ListWidgetItem, but replacing the filePath property used by the filenav by the Exec property of the .desktop ! :D
    // it's necessary to pass this information to the slot called when the user selects its application, that does'nt know anything about it
    ListWidgetItem *it = new ListWidgetItem(false, s.value("Exec").toString().replace("%u", m_file_path).replace("%U", m_file_path), m_list);
    it->setText(name);
    it->setIcon(ic);
    m_list->addItem(it);

    // close the QSettings
    s.endGroup();
}

void OpenAsDialog::m_launch()
{
    if(!m_list->selectedItems().count() > 0)
        return;     // why did this f*ckin user clicked, if he just didn't select anything ?!

    // there shouldn't be more than 1 item selected, get it
    ListWidgetItem *it = static_cast<ListWidgetItem*>(m_list->selectedItems().first());

    // i love this class and my property system ! :)
    QString cmd = it->filePath().path();
    QProcess p(this);
    p.startDetached(cmd);

    // close this dialog
    close();
}
