#-------------------------------------------------
#
# Project created by QtCreator 2010-12-28T01:15:00
#
#-------------------------------------------------

QT       += core gui

TARGET = filenav
TEMPLATE = app

# use glib and nebulae lib
INCLUDEPATH += . \
    /usr/include/glib-2.0/ \
    /usr/lib/glib-2.0/include/ \
    /usr/include/nebulae
LIBS += -lgvfscommon -lnebulae

SOURCES += main.cpp\
        mainwindow.cpp \
    filesystemview.cpp \
    listwidgetitem.cpp \
    contextmenu.cpp \
    openasdialog.cpp \
    fileiconprovider.cpp \
    nebulae-io.cpp

HEADERS  += mainwindow.h \
    filesystemview.h \
    listwidgetitem.h \
    openasdialog.h \
    fileiconprovider.h \
    nebulae-io.h

OTHER_FILES += \
    extensions.conf

FORMS +=
