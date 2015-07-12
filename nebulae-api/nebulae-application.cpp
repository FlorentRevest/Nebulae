#include "nebulae-application.h"
#include "nebulae-basedir.h"

#include <QtCore/QDir>
#include <QtCore/QLocale>
#include <QtCore/QTranslator>

#include <QtDebug>

NebulaeApplication::NebulaeApplication(int argc, char **argv, const QString &name) : QApplication(argc, argv)
{
    setApplicationName(name);

    m_db = new MimeTypeDatabase();

    // creates the configuration folders, if don't exists
    if(!QDir(XDG_DATA_HOME + "/nebulae/" + name).exists())
        QDir(XDG_DATA_HOME).mkpath("/nebulae/" + name);
    if(!QDir(XDG_CONFIG_HOME + "/nebulae/" + name).exists())
        QDir(XDG_CONFIG_HOME).mkpath("/nebulae/" + name);

    // load translators
    QTranslator t;
    QString translation = "/usr/share/nebulae/" + name + "/translations/translation_" + QLocale::system().name().split('_').first();
    bool a = t.load(translation);
    qDebug() << translation << a;
    this->installTranslator(&t);
}
