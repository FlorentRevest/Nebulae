#include <QtGui/QApplication>
#include "mainwindow.h"

#include "nebulae-application.h"

int main(int argc, char *argv[])
{
    NebulaeApplication a(argc, argv, "program-chooser");
    MainWindow w;
    w.show();
    return a.exec();
}
