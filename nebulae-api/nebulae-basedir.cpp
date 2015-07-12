/*
 * This file is part of Nebulae.
 * Copyright (C) 2011 - Leo Testard <leo.testard@gmail.com>
 *
 * Nebulae is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nebulae is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Nebulae. If not, see <http://www.gnu.org/licenses/>.
 */

#include "nebulae-basedir.h"

QString _freedesktop_default_dir_(const QString &varName, bool setVar)
{
    // function to use if the var is not set, to use the default value, and set it
    QString ret;
    if(varName == "XDG_DATA_HOME")
        ret = QDir::homePath() + "/.local/share";
    else if(varName == "XDG_DATA_DIRS")
        ret = "/usr/local/share:/usr/share";
    else if(varName == "XDG_CONFIG_HOME")
        ret = QDir::homePath() + "/.config/";
    else if(varName == "XDG_CONFIG_DIRS")
        ret = "/etc/xdg";
    if(!ret.isEmpty() && setVar)
        setenv(varName.toStdString().c_str(), ret.toStdString().c_str(), 1);

    return ret;
}

// read the value from the environnement variables
QString _get_env_var_(const QString &varName)
{
    QString ret;
    ret = QString::fromUtf8(getenv(varName.toStdString().c_str()));

    if(ret.isEmpty()) // if not set, use the default, and set the env var
        ret = _freedesktop_default_dir_(varName, true);

    return ret;
}
