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

#include "clock.h"

Clock::Clock(QWidget *parent) : QLabel(parent) // This element of the panel is a simple digital clock
{
    setAlignment(Qt::AlignCenter);

    settings = new QSettings("Desktop", "Clock");

    m_color = settings->value("color", QColor(Qt::white)).value<QColor>();
    QPalette palette;
    palette.setBrush(QPalette::WindowText, m_color);
    setPalette(palette);

    m_font = settings->value("font", font()).value<QFont>();
    setFont(m_font);

    m_timer = new QTimer(this);
    m_timer->start(60000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    updateTime();

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(showMenu()));
}

void Clock::updateTime() // Called every minute, it updates the time
{
    setText(QDateTime::currentDateTime().toString("h:mm"));
}

void Clock::showMenu() // It show a menu when the user does a right click
{
    QMenu *menu = new QMenu();
    menu->addAction(QIcon::fromTheme(""), tr("Change font"), this, SLOT(changeFont()));
    menu->addAction(QIcon::fromTheme("applications-graphics"), tr("Change color"), this, SLOT(changeFontColor()));
    menu->popup(QCursor::pos());
}

void Clock::changeFont() // It allows to use another font
{
    bool ok = false;
    m_font = QFontDialog::getFont(&ok, m_font, this);
    if(ok) // If the window isn't canceled
    {
        setFont(m_font);
        settings->setValue("font", m_font);
    }
}

void Clock::changeFontColor() // It allows to use another color
{
    QColor color = QColorDialog::getColor(m_color, this);
    if(color.isValid()) // If the window isn't canceled
    {
        m_color = color;
        settings->setValue("color", m_color);
        QPalette palette;
        palette.setBrush(QPalette::WindowText, m_color);
        setPalette(palette);
    }
}
