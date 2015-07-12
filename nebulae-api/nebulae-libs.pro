# -------------------------------------------------
# Project created by QtCreator 2011-01-30T16:23:05
# -------------------------------------------------
TARGET = nebulae
TEMPLATE = lib
DEFINES += NEBULAELIBS_LIBRARY
QT += xml
SOURCES += \
	nebulae-application.cpp \
    nebulae-desktopentry.cpp \
    nebulae-recentfiles.cpp \
    nebulae-mimetypeinfo.cpp \
    nebulae-basedir.cpp \
    nebulae-freedesktopsettings.cpp
HEADERS += \
	nebulae-application.h \
    nebulae-basedir.h \
    nebulae-translation.h \
    nebulae-desktopentry.h \
    nebulae-recentfiles.h \
    nebulae-mimetypeinfo.h \
    nebulae-freedesktopsettings.h

# use glib and nebulae lib
INCLUDEPATH += . \
    /usr/include/glib-2.0/ \
    /usr/lib/glib-2.0/include/
LIBS += -lgvfscommon

target.path=/usr/lib
includes.files += *.h
includes.path = /usr/include/nebulae/

INSTALLS += target includes
