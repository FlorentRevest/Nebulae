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

#include "windowbutton.h"

WindowButton::WindowButton(Window w) : QWidget()
{
    win = w;
    dpy = QX11Info::display();
    root = QX11Info::appRootWindow();
    xmgr = X11Manager::instance();
    m_active = false;
    m_minimized = false;
    setMinimumSize(300, 320);
}

void WindowButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, height(), height(), QPixmap::grabWindow(win, 0, 0, width()));
    painter.drawText(QRect(0, 300, width(), 20, 0), Qt::AlignCenter,  m_name);
    updateName();
    updateIcon();
}

void WindowButton::mousePressEvent(QMouseEvent *)
{
    if(m_minimized)
    {
        XEvent event;
        event.xclient.type = ClientMessage;
        event.xclient.serial = 0;
        event.xclient.send_event = true;
        event.xclient.message_type = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", false);
        event.xclient.window = win;
        event.xclient.format = 32;
        event.xclient.data.l[0] = 0;
        event.xclient.data.l[1] = 0;
        event.xclient.data.l[2] = 0;
        event.xclient.data.l[3] = 0;
        event.xclient.data.l[4] = 0;

        XSendEvent(dpy, root, False, SubstructureRedirectMask | SubstructureNotifyMask, &event);
        m_minimized = false;
    }
    else
    {
        XIconifyWindow(dpy, win, 0);
        m_minimized = true;
    }
}

void WindowButton::updateName()
{
    m_name = xmgr->name(win);
}

void WindowButton::updateIcon()
{
    m_icon = xmgr->icon(win);
}

void WindowButton::updateAttention()
{

}

void WindowButton::enterEvent(QEvent *)
{
    m_mouseIn = true;
    repaint();
}

void WindowButton::leaveEvent(QEvent *)
{
    m_mouseIn = false;
    repaint();
}

void WindowButton::setActive(bool b)
{
    m_active = b;
}
