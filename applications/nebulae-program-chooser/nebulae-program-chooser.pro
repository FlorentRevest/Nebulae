#-------------------------------------------------
#
# Project created by QtCreator 2011-01-29T00:06:42
#
#-------------------------------------------------

TARGET = nebulae-program-chooser
TEMPLATE = app

INCLUDEPATH += . \
        /usr/include/nebulae
LIBS += -lnebulae

SOURCES += main.cpp\
        mainwindow.cpp \
    programdefaultseditor.cpp

HEADERS  += mainwindow.h \
    programdefaultseditor.h
