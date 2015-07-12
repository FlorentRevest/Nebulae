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

#include "panel/panel.h"

Panel::Panel(QWidget *parent) : QWidget(parent) // This widget is placed at the top of the screen, it contains some widgets
{
    setContextMenuPolicy(Qt::NoContextMenu);
    resize(parent->width(), 32);

    ulong screenReservation[12];
    memset(screenReservation, 0, sizeof(screenReservation));
    screenReservation[2] = 32; //Reserve 32 pixel at the top of the screen
    screenReservation[8] = 0;
    screenReservation[9] = width();

    XChangeProperty(QX11Info::display(), parentWidget()->winId(), XInternAtom(QX11Info::display(), "_NET_WM_STRUT_PARTIAL", false), XA_CARDINAL, 32, PropModeReplace,  (unsigned char *) screenReservation, 12);
    XChangeProperty(QX11Info::display(), parentWidget()->winId(), XInternAtom(QX11Info::display(), "_NET_WM_STRUT", false), XA_CARDINAL, 32, PropModeReplace,  (unsigned char *) screenReservation, 4);

    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(2, 0, 2, 0);

    m_menuSwitcher = new MenuSwitcher();
    m_layout->addWidget(m_menuSwitcher);
    m_taskBar = new TaskBar();
    connect(m_taskBar, SIGNAL(hideMenuSwitcher(bool)), m_menuSwitcher, SLOT(setHidden(bool)));
    m_layout->addWidget(m_taskBar);
    m_systemTray = new SystemTray();
    m_layout->addWidget(m_systemTray);
    m_clock = new Clock();
    m_layout->addWidget(m_clock);
}
