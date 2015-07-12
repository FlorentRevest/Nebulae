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

#include "systemtray.h"

SystemTray::SystemTray(QWidget *parent) : QWidget(parent) // This widget is part of the panel, it shows some application's icon like NetworkManager, Xound...
{
    m_layout = new QHBoxLayout(this);

    systrayOpcode = XInternAtom (QX11Info::display(), "_NET_SYSTEM_TRAY_OPCODE", false);
    Atom net_sys_tray = XInternAtom(QX11Info::display(), QString("_NET_SYSTEM_TRAY_S%1").arg(QX11Info::appScreen()).toLatin1(), false);
    XSetSelectionOwner(QX11Info::display(), net_sys_tray, topLevelWidget()->winId(), CurrentTime);
    XFlush(QX11Info::display());

    XEvent ev; //Send an event, we are now the systray's manager
    memset(&ev, 0, sizeof(ev));
    ev.xclient.type = ClientMessage;
    ev.xclient.message_type = systrayOpcode;
    ev.xclient.format = 32;
    ev.xclient.window = topLevelWidget()->winId();
    ev.xclient.data.l[0] = CurrentTime;
    ev.xclient.data.l[1] = net_sys_tray;
    ev.xclient.data.l[2] = topLevelWidget()->winId();
    XSendEvent(QX11Info::display(), QX11Info::appRootWindow(), false, StructureNotifyMask, &ev);
}

void SystemTray::event(XEvent *event) // This method filters the globals XEvent
{
    if(event->type == ClientMessage && event->xclient.message_type == systrayOpcode && event->xclient.data.l[1] == 0) //A new application wants a place
    {
        QX11EmbedContainer *cli = new QX11EmbedContainer(this);
        cli->embedClient(event->xclient.data.l[2]);
        QPalette p = cli->palette();
        p.setBrush(QPalette::Window, Qt::transparent);
        cli->setPalette(p);
        connect(cli, SIGNAL(clientClosed()), this, SLOT(clientClosed()));

        m_layout->addWidget(cli);
    }
}

void SystemTray::clientClosed() // Called when an icon is closed, it removes the container of the layout
{
    m_layout->removeWidget(static_cast<QX11EmbedContainer *>(sender()));
}
