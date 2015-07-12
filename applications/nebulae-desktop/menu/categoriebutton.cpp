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

#include "menu/categoriebutton.h"

CategorieButton::CategorieButton(QPixmap icon, QString name, QWidget *parent) : QWidget(parent) // This widget shows the icon and name of a menu's categorie
{
    resize(100, 100);
    isHovered = false;

    m_iconLabel = new QLabel(this);
    m_iconLabel->resize(size());
    m_iconLabel->setAlignment(Qt::AlignCenter);
    QPixmap modifiedIcon(48, 100);
    modifiedIcon.fill(Qt::transparent);
    QPainter p(&modifiedIcon);
    p.drawPixmap(0, 2, 48, 48, icon);
    QTransform t;
    p.drawPixmap(0, 50, 48, 48, icon.transformed(t.rotate(180, Qt::XAxis), Qt::SmoothTransformation));
    QLinearGradient alphaGradient(0, 0, 0, height());
    alphaGradient.setColorAt(0.0, Qt::black);
    alphaGradient.setColorAt(0.5, Qt::black);
    alphaGradient.setColorAt(0.8, Qt::transparent);
    QGraphicsOpacityEffect *iconEffect = new QGraphicsOpacityEffect(this);
    iconEffect->setOpacity(1);
    iconEffect->setOpacityMask(alphaGradient);
    m_iconLabel->setPixmap(modifiedIcon);
    m_iconLabel->setGraphicsEffect(iconEffect);

    m_nameLabel = new QLabel(name, this);
    m_nameLabel->setStyleSheet("color: white;");
    QGraphicsDropShadowEffect *nameEffect = new QGraphicsDropShadowEffect();
    nameEffect->setOffset(1);
    nameEffect->setColor(Qt::black);
    m_nameLabel->setGraphicsEffect(nameEffect);
    m_nameLabel->setAlignment(Qt::AlignCenter);
    m_nameLabel->setGeometry(0, 60, width(), m_nameLabel->height());
}

void CategorieButton::enterEvent(QEvent *) // This method is called when the user put the mouse on this widget
{
    isHovered = true;
    repaint();
    emit hover();
}

void CategorieButton::leaveEvent(QEvent *) // This method is called when the mouse leaves the widget
{
    if(mapFromGlobal(QCursor::pos()).y() < 100)
    {
        isHovered = false;
        repaint();
        emit leave();
    }
}

void CategorieButton::paintEvent(QPaintEvent *) // When the mouse is over this widget, we paint a white background
{    
    if(isHovered)
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        QLinearGradient gradient(0, 0, 0, height());
        gradient.setColorAt(0.0, QColor(255, 255, 255, 120));
        gradient.setColorAt(1.0, Qt::white);
        p.setBrush(gradient);
        p.drawRoundedRect(0, 0, width(), height() + 20, 10, 10);
    }
}
