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

#ifndef TASKBAR_H
#define TASKBAR_H

#include <QWidget>
#include <QHash>
#include <QX11Info>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

class TaskBar : public QWidget
{
    Q_OBJECT
public:
    TaskBar(QWidget *parent = 0);
    void updateWinList();
    bool mustBeAdded(Window win);

private:
    QHash<Window, QWidget *> *winHash;
    Display *dpy;
    Window root;

signals:
    void hideMenuSwitcher(bool);

public slots:
    void event(XEvent *);
};

#endif // TASKBAR_H
