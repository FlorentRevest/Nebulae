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

TaskBar::TaskBar(QWidget *parent) : QWidget(parent)
{
    resize(300, qApp->desktop()->height());
    setMaximumSize(300, qApp->desktop()->height());
    setAttribute(Qt::WA_X11NetWmWindowTypeDock);
    dpy = QX11Info::display();
    root = QX11Info::appRootWindow();
    m_winHash = new QHash<Window, WindowButton*>();

    m_xmgr = X11Manager::instance();
    connect(m_xmgr, SIGNAL(X11Event(XEvent*)), this, SLOT(newXEvent(XEvent*)));

    m_layout = new QVBoxLayout(this);
    m_layout->setMargin(0);

    m_space = new QSpacerItem(height(), 0);
    m_layout->addItem(m_space);

    ulong desired_strut[12]; // Reserves 32 pixels at the top of the screen
    memset(desired_strut, 0, sizeof(desired_strut));
    desired_strut[0] = width();
    desired_strut[4] = height();

    XChangeProperty(dpy, winId(), m_xmgr->atom("_NET_WM_STRUT_PARTIAL"), XA_CARDINAL, 32, PropModeReplace, (unsigned char *) desired_strut, 12);
    XChangeProperty(dpy, winId(), m_xmgr->atom("_NET_WM_STRUT"), XA_CARDINAL, 32, PropModeReplace, (unsigned char *) desired_strut, 4);

    m_activeButton = 0;
    updateButtons();
    updateActiveButton();
}

void TaskBar::updateButtons()
{
    Atom type_ret;
    int format_ret;
    unsigned long after_ret, count;
    unsigned char *data = 0;
    XGetWindowProperty(dpy, root, m_xmgr->atom("_NET_CLIENT_LIST"), 0, 0x7fffffff, false, AnyPropertyType, &type_ret, &format_ret, &count, &after_ret, &data);

    Window *wins = (Window *)data;
    for(int i = 0 ; i < count ; i++) // Adds new windows
    {
        Window win = wins[i];
        if(!m_winHash->keys().contains(win))
        {
            if(mustBeAdded(win))
            {
                WindowButton *wb = new WindowButton(win);
                wb->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
                m_layout->addWidget(wb);
                if(m_activeButton == 0)
                {
                    m_layout->removeItem(m_space);
                    m_layout->addItem(m_space);
                }
                m_winHash->insert(win, wb);
            }
        }
    }
    foreach(Window w, m_winHash->keys()) // Removes old windows
    {
        bool isContained = false;
        for(int i = 0 ; i < count ; i++)
        {
            if(wins[i] == w)
                isContained = true;
        }
        if(!isContained)
        {
            if(m_winHash->value(w) == m_activeButton)
                m_activeButton = 0;
            delete m_winHash->value(w);
            m_winHash->remove(w);
        }
    }
}

void TaskBar::updateActiveButton()
{
    if(m_activeButton != 0)
        m_activeButton->setActive(false);
    Atom type_ret;
    int format_ret;
    unsigned long after_ret, count;
    unsigned char *data = 0;
    XGetWindowProperty(dpy, root, m_xmgr->atom("_NET_ACTIVE_WINDOW"), 0, 0x7fffffff, false, AnyPropertyType, &type_ret, &format_ret, &count, &after_ret, &data);

    Window *windowData = (Window*)data;
    Window w = windowData[0];

    WindowButton *b = m_winHash->value(w);
    if(b != 0)
    {
        m_activeButton = b;
        m_activeButton->setActive(true);
        m_layout->removeItem(m_space);
    }
    else
    {
        m_activeButton = 0;
        m_layout->addItem(m_space);
    }
}

void TaskBar::newXEvent(XEvent *e) // Checks if an info has been changed (about a name, an icon, the clients list etc...
{
    if(e->type == PropertyNotify)
    {
        if(e->xproperty.atom == m_xmgr->atom("_NET_CLIENT_LIST") && e->xproperty.window == root)
            updateButtons();

        else if(e->xproperty.atom == m_xmgr->atom("_NET_ACTIVE_WINDOW"))
            updateActiveButton();

        else if(e->xproperty.atom == m_xmgr->atom("_NET_WM_NAME") && m_winHash->keys().contains(e->xproperty.window))
            m_winHash->value(e->xproperty.window)->updateName();

        else if(e->xproperty.atom == m_xmgr->atom("_NET_WM_ICON") && m_winHash->keys().contains(e->xproperty.window))
            m_winHash->value(e->xproperty.window)->updateIcon();

        else if(e->xproperty.atom == m_xmgr->atom("WM_HINTS") && m_winHash->keys().contains(e->xproperty.window))
            m_winHash->value(e->xproperty.window)->updateAttention();
    }
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
