#include "mainwindow.h"

#include <QtCore/QDir>
#include <QtCore/QLocale>
#include <QtCore/QFileInfo>
#include <QtCore/QTranslator>

#include <QtGui/QApplication>

#include <QtDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // install a translated version
    QTranslator translator;
    QString locale = QLocale::system().name().split('_').first();
    if(QFile("/usr/share/nebulae/image-viewer/translations/imageviewer_" + locale + ".qm").exists())
        translator.load("/usr/share/nebulae/image-viewer/translations/imageviewer_" + locale + ".qm");
    else if(QFile(app.applicationDirPath() + "/imageviewer_" + locale + ".qm").exists())
        translator.load("imageviewer_" + locale);
    app.installTranslator(&translator);

    // the first argc is the path of an image
    QString image_path = (argc > 1) ? argv[1] : QString();
    QString dir_path;
    if(!image_path.isEmpty())    // get the dir of the picture
        dir_path = QFileInfo(image_path).absoluteDir().absolutePath();
    else    // use some default path
        dir_path = QDir::homePath() + "/" + app.tr("Pictures");

    // the main window needs a dir path, but the image is facultative
    MainWindow m(dir_path, QFileInfo(image_path).fileName());
	m.resize(400,450);
	m.show();

    // Starts the Qt application
    return app.exec();
}
