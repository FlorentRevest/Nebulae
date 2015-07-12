/*Copyright (C) 2011 Revest Florent <florent.revest666@gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef DESKTOP_H
#define DESKTOP_H

#include <QWidget>
#include <QApplication>
#include <QSettings>
#include <QMenu>
#include <QPainter>
#include <QDesktopWidget>
#include <QContextMenuEvent>

#include "wallpaperchooser.h"
#include "menu/categoriebutton.h"
#include "menu/shutdownbutton.h"
#include "menu/searchbar.h"
#include "menu/menucontainer.h"
#include "panel/panel.h"

class Desktop : public QWidget
{
    Q_OBJECT

public:
    Desktop(QWidget *parent = 0);

public slots:
    void showWall(QString);
    void changeStyle(Style);
    void changeColor(QColor);
    void showWallChooser();
    void hideWallChooser();

private:
    QSettings *settings;
    QMenu *menu;
    WallpaperChooser *wallc;
    QString wallURL;
    QLabel *label;
    Style m_s;
    Panel *m_panel;
    ShutdownButton *m_shutdownButton;
    SearchBar *m_searchBar;
    MenuContainer *m_menuContainer;

protected:
    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // DESKTOP_H
