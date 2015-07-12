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

#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include <QPainter>
#include <QList>
#include <QIcon>
#include <QBitmap>

struct ChartPart
{
    float percentage;
    QString name;
    QIcon icon;
    QBitmap bitmap;
    QColor color;
};

class Chart : public QWidget
{
    Q_OBJECT

public:
    Chart(QWidget *parent = 0);
    void addPartition(float percentage, QString name, QIcon icon);

private:
    QList<QColor> m_colorList;
    QList<ChartPart *> m_partList;

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
};

#endif // CHART_H
