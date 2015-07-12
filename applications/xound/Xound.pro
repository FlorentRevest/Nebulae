##########################################################################
# Copyright (C) 2010 Revest Florent <florent.revest666@gmail.com>        #
##
# This program is free software; you can redistribute it and/or modify   #
# it under the terms of the GNU General Public License as published by   #
# the Free Software Foundation; either version 2 of the License, or      #
# (at your option) any later version.                                    #
##
# This program is distributed in the hope that it will be useful,        #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
# GNU General Public License for more details.                           #
##
# You should have received a copy of the GNU General Public License along#
# with this program; if not, write to the Free Software Foundation, Inc.,#
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.            #
# Information about the application and its dependencies                 #
##########################################################################
TARGET = xound
TEMPLATE = app
QT += network \
    xml \
    phonon \
    dbus
LIBS += -ltag -lffmpegthumbnailer
OBJECTS_DIR = obj
MOC_DIR = obj
include(libs/qtsingleapplication/src/qtsingleapplication.pri)
SOURCES += main.cpp \
    gui/mainwindow.cpp \
    gui/musiccontrol.cpp \
    global/core.cpp \
    global/artist.cpp \
    global/album.cpp \
    global/playlist.cpp \
    global/music.cpp \
    global/mprismanager.cpp \
    gui/video.cpp \
    gui/workzone.cpp \
    gui/artistwidget.cpp \
    gui/playlistwidget.cpp \
    gui/cover.cpp \
    gui/musiclibrary.cpp \
    gui/musicnote.cpp \
    global/infothread.cpp \
    gui/videolibrary.cpp
HEADERS += include/mainwindow.h \
    include/core.h \
    include/artist.h \
    include/album.h \
    include/playlist.h \
    include/musiccontrol.h \
    include/music.h \
    include/video.h \
    include/contentplugin.h \
    include/workzone.h \
    include/artistwidget.h \
    include/playlistwidget.h \
    include/cover.h \
    include/musiclibrary.h \
    include/mprismanager.h \
    include/musicnote.h \
    include/infothread.h \
    include/videolibrary.h
RESOURCES += images/ressource.qrc
TRANSLATIONS = translations/Xound_fr.ts
target.path = /usr/bin/
icon.files = installation/xound.png
icon.path = /usr/share/icons/
desktop.files = Xound.desktop
desktop.path = /usr/share/applications/
INSTALLS += target \
    icon \
    desktop
