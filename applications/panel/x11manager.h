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

#ifndef X11MANAGER_H
#define X11MANAGER_H

#include <QObject>
#include <QHash>
#include <QX11Info>
#include <QPixmap>
#include <QBitmap>

extern "C" {
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
}

class X11Manager : public QObject
{
    Q_OBJECT

public:
    static X11Manager *instance();
    void newX11Event(XEvent *);
    Atom atom(QString);
    QString name(Window);
    QPixmap icon(Window);

private:
    X11Manager(); // There is a private constructor because X11Manager is a singleton class

    QHash<QString, Atom> m_registeredAtoms;
    Display *m_dpy;
    Window m_root;

signals:
    void X11Event(XEvent*);
};

#endif // X11MANAGER_H
