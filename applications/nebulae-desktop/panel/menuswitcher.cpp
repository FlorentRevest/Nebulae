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

#include "menuswitcher.h"

MenuSwitcher::MenuSwitcher(QWidget *parent) : QLabel(parent) // This little button is placed at the left of the panel, it allows to switch between the desktop
{
    setText(tr("Menu"));
    setStyleSheet("color: white");
    menuShown = false;
}

void MenuSwitcher::mousePressEvent(QMouseEvent *) // When the button is clicked, shows the desktop or reshows the windows
{
    XEvent xev;

    xev.xclient.type = ClientMessage;
    xev.xclient.serial = 0;
    xev.xclient.send_event = true;
    xev.xclient.display = QX11Info::display();
    xev.xclient.window = QX11Info::appRootWindow();
    xev.xclient.message_type = XInternAtom(QX11Info::display(), "_NET_SHOWING_DESKTOP", false);
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = !menuShown;
    xev.xclient.data.l[1] = 0;
    xev.xclient.data.l[2] = 0;
    xev.xclient.data.l[3] = 0;
    xev.xclient.data.l[4] = 0;

    XSendEvent(QX11Info::display(), QX11Info::appRootWindow(), false, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
    menuShown = !menuShown;
    if(menuShown) // If we show the desktop(menu), change the text to "Applications"
        setText(tr("Applications"));
    else // If we show the windows, change the text to "Menu"
        setText(tr("Menu"));
}
