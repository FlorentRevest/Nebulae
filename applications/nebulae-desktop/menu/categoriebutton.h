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

#ifndef CATEGORIEBUTTON_H
#define CATEGORIEBUTTON_H

#include <QWidget>
#include <QPainter>
#include <QIcon>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QGraphicsDropShadowEffect>

class CategorieButton : public QWidget
{
    Q_OBJECT
public:
    CategorieButton(QPixmap icon, QString name, QWidget *parent = 0);

private:
    bool isHovered;
    QLabel *m_nameLabel, *m_iconLabel;

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);

signals:
    void hover();
    void leave();
};

#endif // CATEGORIEBUTTON_H
