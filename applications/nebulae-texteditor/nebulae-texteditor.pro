# -------------------------------------------------
# Project created by QtCreator 2010-05-02T19:27:34
# -------------------------------------------------
TARGET = nebulae-texteditor 
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    editorwidget.cpp \
    actions.cpp \
    tabwidget.cpp \
    options.cpp \
    chighlighter.cpp
HEADERS += mainwindow.h \
    editorwidget.h \
    tabwidget.h \
    chighlighter.h
TRANSLATIONS = TextEditor_fr.ts

INCLUDEPATH += . \
            /usr/include/nebulae
LIBS += -lnebulae

translators.files = *.qm
translators.path = /usr/share/nebulae/texteditor/translations/
target.path = /usr/bin

INSTALLS += target translators
