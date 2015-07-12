#-------------------------------------------------
#
# Project created by QtCreator 2011-01-27T22:54:59
#
#-------------------------------------------------

QT       += core gui

TARGET = nebulae-admin-panel
TEMPLATE = app
INCLUDEPATH += . \
        /usr/include/nebulae/

LIBS += -lnebulae

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    searchlineedit.h

TRANSLATIONS += translation_fr.ts

target.path = /usr/bin/
translations.files = *.qm
translations.path = /usr/share/nebulae/admin-panel/translations/

INSTALLS += target translations
