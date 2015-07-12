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

#include "menucontainer.h"

MenuContainer::MenuContainer(QWidget *parent) : QWidget(parent)
{
}

void MenuContainer::enterEvent(QEvent*)
{
    show();
}

void MenuContainer::leaveEvent(QEvent *)
{
    hide();
}

void MenuContainer::showItems()
{
    show();
}

void MenuContainer::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setBrush(Qt::white);
    p.drawRoundedRect(0, 0, width(), height(), 10, 10);
}
