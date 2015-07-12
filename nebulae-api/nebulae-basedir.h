#ifndef FREEDESKTOPBASEDIRECTORIES_H
#define FREEDESKTOPBASEDIRECTORIES_H

extern "C" {
    #include <stdlib.h>
}

/**
  * \file nebulae-basedir.h
  *
  * Contains an implementation of the Freedesktop BaseDir specification \link http://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html
  *
  * This implementation returns the dirs from the environnement variables, and, if not set, returns the default values, and set the variable to it.
  * The dirs are accessible via defines like XDG_DATA_HOME, that returns as example the $XDG_DATA_HOME path, by default $HOME/.local/share/
  *
  */

#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QStringList>

/**
  * \def XDG_DATA_HOME
  * Returns a dir where the application can store data in the user's home directory. The stored data is specific to this user. Default is $HOME/.local/share
  */
#define XDG_DATA_HOME   _get_env_var_("XDG_DATA_HOME")

/**
  * \def XDG_DATA_DIRS
  * Returns a list of paths where the application can store data. The stored data is not specific to any user, and is system-wide. Defaults are /usr/local/share and /usr/share
  */
#define XDG_DATA_DIRS   _get_env_var_("XDG_DATA_DIRS").split(":")

/**
  * \def XDG_CONFIG_HOME
  * Returns a path where the application can store user-specific configuration (setting files, etc). Default is $HOME/.config/
  */
#define XDG_CONFIG_HOME _get_env_var_("XDG_CONFIG_HOME")

/**
  * \def XDG_CONFIG_DIRS
  * Returns a list of paths where the application can store system-wide configuration. Default is /etc/xdg
  */
#define XDG_CONFIG_DIRS _get_env_var_("XDG_CONFIG_DIRS").split(":")

/** \def HOME
  * Returns the home path of the current user, in most cases /home/user/
  */
#define HOME QDir::homePath()

/** \def PATH
  * Returns a list of the directories from where binaries can be launched without giving the full path
  */
#define PATH _get_env_var_("PATH").split(":")

QString _freedesktop_default_dir_(const QString &varName, bool setVar);

// read the value from the environnement variables
QString _get_env_var_(const QString &varName);

#endif // FREEDESKTOPBASEDIRECTORIES_H
