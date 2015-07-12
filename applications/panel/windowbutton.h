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

#ifndef WINDOWBUTTON_H
#define WINDOWBUTTON_H

#include <QWidget>
#include <QPainter>
#include <QX11Info>
#include <QBitmap>
#include <QIcon>
#include <QPointer>
#include <QColor>
#include <QDebug>

#include "x11manager.h"

extern "C" {
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
}

class WindowButton : public QWidget
{
Q_OBJECT
public:
    WindowButton(Window w);
    void updateName();
    void updateIcon();
    void updateAttention();
    void setActive(bool);

private:
    bool m_active, m_minimized, m_attention, m_mouseIn;
    Display *dpy;
    Window root, win;
    QPixmap m_icon;
    QString m_name;
    X11Manager *xmgr;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
};

#endif // WINDOWBUTTON_H
