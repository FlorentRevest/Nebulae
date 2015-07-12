/*Copyright (C) 2010 Revest Florent <florent.revest666@gmail.com>
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

#include "chart.h"

Chart::Chart(QWidget *parent) : QWidget(parent)
{
    m_colorList = QList<QColor>();
    m_colorList << QColor(Qt::red) << QColor(Qt::yellow) << QColor(Qt::blue) << QColor(Qt::green);

    m_partList = QList<ChartPart *>();

    resize(300, 300);
    addPartition(30, "meeeow", QIcon());
    addPartition(60, "meeeow", QIcon());
}

void Chart::addPartition(float percentage, QString name, QIcon icon)
{
    ChartPart *newPart = new ChartPart;
    newPart->percentage = percentage;
    newPart->name = name;
    newPart->icon = icon;
    newPart->color = m_colorList[m_partList.count()];
    m_partList.append(newPart);
}

void Chart::paintEvent(QPaintEvent *) // This method paints the pie
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int startAngle = 0;
    foreach(ChartPart *cp, m_partList)
    {
        QLinearGradient gradient(0, 0, width(), 0);
        gradient.setColorAt(0, cp->color.darker(115));
        gradient.setColorAt(1, cp->color);
        p.setBrush(gradient);
        p.drawPie(0, 0, width(), height(), startAngle, cp->percentage * 57.6);
        startAngle += cp->percentage * 57.6;
    }
}

void Chart::mouseMoveEvent(QMouseEvent *) // This method highlights the portion under the mouse
{

}

