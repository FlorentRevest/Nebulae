#############################################################################
# Makefile for building: desktop
# Generated by qmake (2.01a) (Qt 4.8.7) on: mer. ao�t 26 00:46:28 2015
# Project:  Desktop.pro
# Template: app
# Command: /usr/lib/x86_64-linux-gnu/qt4/bin/qmake -o Makefile Desktop.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DQT_NO_DEBUG -DQT_DBUS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -m64 -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -m64 -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++-64 -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtDBus -I/usr/include/qt4 -I.
LINK          = g++
LFLAGS        = -m64 -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib/x86_64-linux-gnu -lX11 -lQtDBus -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/lib/x86_64-linux-gnu/qt4/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
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
		menu/menucontainer.cpp moc_desktop.cpp \
		moc_wallpaperchooser.cpp \
		moc_categoriebutton.cpp \
		moc_panel.cpp \
		moc_clock.cpp \
		moc_menuswitcher.cpp \
		moc_taskbar.cpp \
		moc_systemtray.cpp \
		moc_searchbar.cpp \
		moc_shutdownbutton.cpp \
		moc_app.cpp \
		moc_menucontainer.cpp
OBJECTS       = main.o \
		desktop.o \
		wallpaperchooser.o \
		categoriebutton.o \
		panel.o \
		clock.o \
		menuswitcher.o \
		taskbar.o \
		systemtray.o \
		searchbar.o \
		shutdownbutton.o \
		app.o \
		menucontainer.o \
		moc_desktop.o \
		moc_wallpaperchooser.o \
		moc_categoriebutton.o \
		moc_panel.o \
		moc_clock.o \
		moc_menuswitcher.o \
		moc_taskbar.o \
		moc_systemtray.o \
		moc_searchbar.o \
		moc_shutdownbutton.o \
		moc_app.o \
		moc_menucontainer.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/dbusinterfaces.prf \
		/usr/share/qt4/mkspecs/features/dbusadaptors.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		Desktop.pro
QMAKE_TARGET  = desktop
DESTDIR       = 
TARGET        = desktop

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: Desktop.pro  /usr/share/qt4/mkspecs/linux-g++-64/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/dbusinterfaces.prf \
		/usr/share/qt4/mkspecs/features/dbusadaptors.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/x86_64-linux-gnu/libQtDBus.prl \
		/usr/lib/x86_64-linux-gnu/libQtGui.prl \
		/usr/lib/x86_64-linux-gnu/libQtCore.prl
	$(QMAKE) -o Makefile Desktop.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/shared.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/dbusinterfaces.prf:
/usr/share/qt4/mkspecs/features/dbusadaptors.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/x86_64-linux-gnu/libQtDBus.prl:
/usr/lib/x86_64-linux-gnu/libQtGui.prl:
/usr/lib/x86_64-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile Desktop.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/desktop1.0.0 || $(MKDIR) .tmp/desktop1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/desktop1.0.0/ && $(COPY_FILE) --parents desktop.h wallpaperchooser.h menu/categoriebutton.h panel/panel.h panel/clock.h panel/menuswitcher.h panel/taskbar.h panel/systemtray.h menu/searchbar.h menu/shutdownbutton.h app.h menu/menucontainer.h .tmp/desktop1.0.0/ && $(COPY_FILE) --parents main.cpp desktop.cpp wallpaperchooser.cpp menu/categoriebutton.cpp panel/panel.cpp panel/clock.cpp panel/menuswitcher.cpp panel/taskbar.cpp panel/systemtray.cpp menu/searchbar.cpp menu/shutdownbutton.cpp app.cpp menu/menucontainer.cpp .tmp/desktop1.0.0/ && (cd `dirname .tmp/desktop1.0.0` && $(TAR) desktop1.0.0.tar desktop1.0.0 && $(COMPRESS) desktop1.0.0.tar) && $(MOVE) `dirname .tmp/desktop1.0.0`/desktop1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/desktop1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_desktop.cpp moc_wallpaperchooser.cpp moc_categoriebutton.cpp moc_panel.cpp moc_clock.cpp moc_menuswitcher.cpp moc_taskbar.cpp moc_systemtray.cpp moc_searchbar.cpp moc_shutdownbutton.cpp moc_app.cpp moc_menucontainer.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_desktop.cpp moc_wallpaperchooser.cpp moc_categoriebutton.cpp moc_panel.cpp moc_clock.cpp moc_menuswitcher.cpp moc_taskbar.cpp moc_systemtray.cpp moc_searchbar.cpp moc_shutdownbutton.cpp moc_app.cpp moc_menucontainer.cpp
moc_desktop.cpp: wallpaperchooser.h \
		menu/categoriebutton.h \
		menu/shutdownbutton.h \
		menu/searchbar.h \
		menu/menucontainer.h \
		panel/panel.h \
		panel/clock.h \
		panel/taskbar.h \
		panel/systemtray.h \
		panel/menuswitcher.h \
		desktop.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) desktop.h -o moc_desktop.cpp

moc_wallpaperchooser.cpp: wallpaperchooser.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) wallpaperchooser.h -o moc_wallpaperchooser.cpp

moc_categoriebutton.cpp: menu/categoriebutton.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) menu/categoriebutton.h -o moc_categoriebutton.cpp

moc_panel.cpp: panel/clock.h \
		panel/taskbar.h \
		panel/systemtray.h \
		panel/menuswitcher.h \
		panel/panel.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) panel/panel.h -o moc_panel.cpp

moc_clock.cpp: panel/clock.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) panel/clock.h -o moc_clock.cpp

moc_menuswitcher.cpp: panel/menuswitcher.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) panel/menuswitcher.h -o moc_menuswitcher.cpp

moc_taskbar.cpp: panel/taskbar.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) panel/taskbar.h -o moc_taskbar.cpp

moc_systemtray.cpp: panel/systemtray.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) panel/systemtray.h -o moc_systemtray.cpp

moc_searchbar.cpp: menu/searchbar.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) menu/searchbar.h -o moc_searchbar.cpp

moc_shutdownbutton.cpp: menu/shutdownbutton.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) menu/shutdownbutton.h -o moc_shutdownbutton.cpp

moc_app.cpp: app.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) app.h -o moc_app.cpp

moc_menucontainer.cpp: menu/menucontainer.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) menu/menucontainer.h -o moc_menucontainer.cpp

compiler_dbus_interface_source_make_all:
compiler_dbus_interface_source_clean:
compiler_dbus_adaptor_source_make_all:
compiler_dbus_adaptor_source_clean:
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_dbus_interface_header_make_all:
compiler_dbus_interface_header_clean:
compiler_dbus_interface_moc_make_all:
compiler_dbus_interface_moc_clean:
compiler_dbus_adaptor_header_make_all:
compiler_dbus_adaptor_header_clean:
compiler_dbus_adaptor_moc_make_all:
compiler_dbus_adaptor_moc_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

main.o: main.cpp desktop.h \
		wallpaperchooser.h \
		menu/categoriebutton.h \
		menu/shutdownbutton.h \
		menu/searchbar.h \
		menu/menucontainer.h \
		panel/panel.h \
		panel/clock.h \
		panel/taskbar.h \
		panel/systemtray.h \
		panel/menuswitcher.h \
		app.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

desktop.o: desktop.cpp desktop.h \
		wallpaperchooser.h \
		menu/categoriebutton.h \
		menu/shutdownbutton.h \
		menu/searchbar.h \
		menu/menucontainer.h \
		panel/panel.h \
		panel/clock.h \
		panel/taskbar.h \
		panel/systemtray.h \
		panel/menuswitcher.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o desktop.o desktop.cpp

wallpaperchooser.o: wallpaperchooser.cpp wallpaperchooser.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o wallpaperchooser.o wallpaperchooser.cpp

categoriebutton.o: menu/categoriebutton.cpp menu/categoriebutton.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o categoriebutton.o menu/categoriebutton.cpp

panel.o: panel/panel.cpp panel/panel.h \
		panel/clock.h \
		panel/taskbar.h \
		panel/systemtray.h \
		panel/menuswitcher.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o panel.o panel/panel.cpp

clock.o: panel/clock.cpp panel/clock.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o clock.o panel/clock.cpp

menuswitcher.o: panel/menuswitcher.cpp panel/menuswitcher.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o menuswitcher.o panel/menuswitcher.cpp

taskbar.o: panel/taskbar.cpp panel/taskbar.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o taskbar.o panel/taskbar.cpp

systemtray.o: panel/systemtray.cpp panel/systemtray.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o systemtray.o panel/systemtray.cpp

searchbar.o: menu/searchbar.cpp menu/searchbar.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o searchbar.o menu/searchbar.cpp

shutdownbutton.o: menu/shutdownbutton.cpp menu/shutdownbutton.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o shutdownbutton.o menu/shutdownbutton.cpp

app.o: app.cpp app.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o app.o app.cpp

menucontainer.o: menu/menucontainer.cpp menu/menucontainer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o menucontainer.o menu/menucontainer.cpp

moc_desktop.o: moc_desktop.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_desktop.o moc_desktop.cpp

moc_wallpaperchooser.o: moc_wallpaperchooser.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_wallpaperchooser.o moc_wallpaperchooser.cpp

moc_categoriebutton.o: moc_categoriebutton.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_categoriebutton.o moc_categoriebutton.cpp

moc_panel.o: moc_panel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_panel.o moc_panel.cpp

moc_clock.o: moc_clock.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_clock.o moc_clock.cpp

moc_menuswitcher.o: moc_menuswitcher.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_menuswitcher.o moc_menuswitcher.cpp

moc_taskbar.o: moc_taskbar.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_taskbar.o moc_taskbar.cpp

moc_systemtray.o: moc_systemtray.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_systemtray.o moc_systemtray.cpp

moc_searchbar.o: moc_searchbar.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_searchbar.o moc_searchbar.cpp

moc_shutdownbutton.o: moc_shutdownbutton.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_shutdownbutton.o moc_shutdownbutton.cpp

moc_app.o: moc_app.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_app.o moc_app.cpp

moc_menucontainer.o: moc_menucontainer.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_menucontainer.o moc_menucontainer.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

