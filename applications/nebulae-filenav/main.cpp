#include <QtGui/QApplication>
#include "mainwindow.h"

#include "nebulae/nebulae-application.h"

int main(int argc, char *argv[])
{
    NebulaeApplication a(argc, argv, "filenav");
    MainWindow w;
    w.show();

    return a.exec();
}
