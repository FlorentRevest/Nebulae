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

#include "taskbar.h"

TaskBar::TaskBar(QWidget *parent) : QWidget(parent) // This widget shows the list of windows and allows to manage them
{
    dpy = QX11Info::display();
    root = QX11Info::appRootWindow();
    winHash = new QHash<Window, QWidget *>();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    updateWinList();
}

void TaskBar::updateWinList()
{
    Atom type_ret;
    int format_ret;
    unsigned long after_ret, count;
    unsigned char *data = 0;
    XGetWindowProperty(dpy, root, XInternAtom(dpy, "_NET_CLIENT_LIST", false), 0, 0x7fffffff, false, AnyPropertyType, &type_ret, &format_ret, &count, &after_ret, &data);

    static uint itemsNb = 0;

    Window *wins = (Window *)data;
    for(int i = 0 ; i < count ; i++)
    {
        Window win = wins[i];
        if(!winHash->keys().contains(win))
        {
            if(mustBeAdded(win))
            {
                qDebug(QString::number(win).toAscii());
//                WindowButton *wb = new WindowButton(win, this);
//                winHash->insert(win, wb);
//                wb->move(0, itemsNb * 50);
//                wb->show();
//                itemsNb ++;
            }
        }
    }
    if(itemsNb == 0)
        emit hideMenuSwitcher(true);
    else
        emit hideMenuSwitcher(false);
}

bool TaskBar::mustBeAdded(Window win)
{
    Atom type_ret;
    int format_ret;
    unsigned long after_ret, count;
    Atom *atoms;
    unsigned char **data = (unsigned char **)&atoms;
    XGetWindowProperty(dpy, win, XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", false), 0, 0x7fffffff, false, AnyPropertyType, &type_ret, &format_ret, &count, &after_ret, data);
    for (int i = 0; i < count; i++)
    {
        Atom atm = atoms[i];
        if(atm == XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DESKTOP", false) || atm == XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", false) ||
           atm == XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_TOOLBAR", false) || atm == XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_MENU", false) ||
           atm == XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_SPLASH", false) || atm == XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DROPDOWN", false) ||
           atm == XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_POPUP_MENU", false) || atm == XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_TOOLTIP", false) ||
           atm == XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_NOTIFICATION", false) || atm == XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_COMBO", false) ||
           atm == XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DND", false) || atm == XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_UTILITY", false))
            return false;
    }

    XGetWindowProperty(dpy, win, XInternAtom(dpy, "_NET_WM_STATE", false), 0, 0x7fffffff, false, AnyPropertyType, &type_ret, &format_ret, &count, &after_ret, data);
    for (int i = 0; i < count; i++)
        if (atoms[i] == XInternAtom(dpy, "_NET_WM_STATE_SKIP_TASKBAR", false))
            return false;

    return true;
}

void TaskBar::event(XEvent *e)
{
    // If a window is added or removed, add it to the list!
    if(e->type == PropertyNotify && e->xproperty.window == root && e->xproperty.atom == XInternAtom(dpy, "_NET_CLIENT_LIST", false))
        updateWinList();
}
