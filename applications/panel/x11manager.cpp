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

#include "x11manager.h"

X11Manager *X11Manager::instance() // This is a singleton class, we allow only one instance
{
    static X11Manager *xmgr = new X11Manager();
    return xmgr;
}

X11Manager::X11Manager() : QObject()
{
    m_registeredAtoms = QHash<QString, Atom>();
    m_dpy = QX11Info::display();
    m_root = QX11Info::appRootWindow();
}

Atom X11Manager::atom(QString name) // Manages a cache containing already called Atoms
{
    if(!m_registeredAtoms.contains(name))
        m_registeredAtoms.insert(name, XInternAtom(m_dpy, name.toLatin1(), false));
    return m_registeredAtoms.value(name);
}

void X11Manager::newX11Event(XEvent *e) // Receive a event from App, and forwards it to the other classes
{
    emit X11Event(e);
}

QString X11Manager::name(Window win) // Return the name of the given Window
{
    char *nam;
    char **name = &nam;
    Atom type;
    int format;
    unsigned long nitems=0, bytes_after=0;

    XGetWindowProperty(m_dpy, win, atom("_NET_WM_NAME"), 0, 500, false, AnyPropertyType, &type, &format, &nitems, &bytes_after, (unsigned char **) name);

    if(*name != NULL)
        return QString::fromLocal8Bit(nam);

    XFetchName(m_dpy, win, name);
    return tr("Untitled Window");
}

QPixmap X11Manager::icon(Window win)
{
    Atom type_ret = None;
    unsigned char *data = 0;
    int format = 0, width = 0, height = 0;
    unsigned long n = 0, extra = 0;


    if(XGetWindowProperty(m_dpy, win, atom("_NET_WM_ICON"), 0, 1, False, XA_CARDINAL, &type_ret, &format, &n, &extra, (unsigned char **)&data) == Success)
    {
        if(data)
        {
            width = data[0];
            XFree(data);
            XGetWindowProperty(m_dpy, win, atom("_NET_WM_ICON"), 1, 1, False, XA_CARDINAL, &type_ret, &format, &n, &extra, (unsigned char **)&data);
            height = data[0];
            XFree(data);
            XGetWindowProperty(m_dpy, win, atom("_NET_WM_ICON"), 2, width*height, False, XA_CARDINAL, &type_ret, &format, &n, &extra, (unsigned char **)&data);
            QImage img(data, width, height, QImage::Format_ARGB32);
            XFree(data);
            if(!img.isNull())
                return QPixmap::fromImage(img);
        }
    }
    XFree(data);

    XWMHints *xh;
    if((xh = XGetWMHints(m_dpy, win)) != NULL)
    {
        if(xh->flags & IconPixmapHint)
        {
            GC gc = XCreateGC(m_dpy, xh->icon_mask, 0, 0);
            QPixmap pix = QPixmap::fromX11Pixmap(xh->icon_pixmap);
            QBitmap mask(pix.width(), pix.height());
            mask.detach();
            XCopyArea(m_dpy, xh->icon_mask, mask.handle(), gc, 0, 0, pix.width(), pix.height(), 0, 0);
            pix.setMask(mask);
            return pix;
        }
    }
    return QPixmap("window.png");
}
