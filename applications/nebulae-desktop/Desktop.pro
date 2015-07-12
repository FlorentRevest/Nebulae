# Copyright (C) 2010 Revest Florent <florent.revest666@gmail.com>         #
#                                                                         #
# This program is free software; you can redistribute it and/or modify    #
# it under the terms of the GNU General Public License as published by    #
# the Free Software Foundation; either version 2 of the License, or       #
# (at your option) any later version.                                     #
#                                                                         #
# This program is distributed in the hope that it will be useful,         #
# but WITHOUT ANY WARRANTY; without even the implied warranty of          #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            #
# GNU General Public License for more details.                            #
#                                                                         #
# You should have received a copy of the GNU General Public License along #
# with this program; if not, write to the Free Software Foundation, Inc., #
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.             #


# This file is the configuration of the project used to build the Makefile

TARGET = desktop
TEMPLATE = app
LIBS += -lX11
QT += dbus

SOURCES += main.cpp\
        desktop.cpp \
        wallpaperchooser.cpp \
        menu/categoriebutton.cpp \
        panel/panel.cpp \
        panel/clock.cpp \
        panel/menuswitcher.cpp \
        panel/taskbar.cpp \
        panel/systemtray.cpp \
        menu/searchbar.cpp \
        menu/shutdownbutton.cpp \
        app.cpp \
    menu/menucontainer.cpp

HEADERS  += desktop.h \
            wallpaperchooser.h \
            menu/categoriebutton.h \
            panel/panel.h \
            panel/clock.h \
            panel/menuswitcher.h \
            panel/taskbar.h \
            panel/systemtray.h \
            menu/searchbar.h \
            menu/shutdownbutton.h \
            app.h \
    menu/menucontainer.h
