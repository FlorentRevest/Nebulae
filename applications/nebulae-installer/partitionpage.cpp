/*
 * This file is part of Nebulae.
 * Copyright (C) 2011 - Leo Testard <leo.testard@gmail.com>
 *
 * Nebulae is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nebulae is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Nebulae. If not, see <http://www.gnu.org/licenses/>.
 */

#include "partitionpage.h"
#include "nightcharts.h"

#include <parted/parted.h>

#include <QtGui/QFont>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QPaintEvent>
#include <QtGui/QTreeWidget>
#include <QtGui/QPushButton>

#include <QtDebug>

QColor m_color(int index)
{
    switch(index) {
    case 0:
        return Qt::red;
    case 1:
        return Qt::green;
    case 2:
        return Qt::blue;
    case 3:
        return Qt::yellow;
    case 4:
        return Qt::cyan;
    case 5:
        return Qt::magenta;
    }
}

QIcon m_icon(QString fs)
{
    if(fs.startsWith("ext"))
        return QIcon("/home/leo/coverflow/tux.jpg");
    if(fs == "ntfs" || fs.startsWith("fat"))
        return QIcon("/home/leo/coverflow/vista_logo.png");
}

PartitionPage::PartitionPage(InfoManager *inf, QWidget *parent) : QWidget(parent), m_inf_mgr(inf)
{
    m_title = new QLabel(tr("Partitionning"), this);
    m_title->setAlignment(Qt::AlignCenter);
    QFont f; f.setPixelSize(24);
    m_title->setFont(f);
    m_title->setBackgroundRole(QPalette::Dark);

    m_text = new QLabel(this);
    m_text->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_text->setText(tr("The following diagram represents the actual partitions of you hard drive. To install Nebulae, you must select a partition or an empty area. \n"
                       "Be aware : if you select an existing partition, its contents will be erased. If you want to keep the actual operating system, it is recommanded "
                       "to resize the existing partition and install Nebulae in the empty area."));
    m_text->setWordWrap(true);
    m_text->setContentsMargins(20, 0, 20, 0);
    m_text->setBackgroundRole(QPalette::Dark);

    m_resize = new QPushButton(QIcon("/usr/share/nebulae/installer/icons/resize.png"), tr("Resize"), this);
    m_delete = new QPushButton(QIcon("/usr/share/nebulae/installer/icons/delete.png"), tr("Delete"), this);

    m_dev_box = new QComboBox(this);
    connect(m_dev_box, SIGNAL(currentIndexChanged(QString)), this, SLOT(m_device_changed(QString)));

    m_tree = new QTreeWidget(this);
    m_tree->setIconSize(QSize(32,32));
    m_tree->setColumnCount(2);
    m_tree->setColumnWidth(0, 300);
    m_tree->setHeaderLabels(QStringList() << "Partition" << "Size");
    connect(m_tree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(m_tree_clicked(QTreeWidgetItem*,QTreeWidgetItem*)));

    m_update_parts_list();
}

void PartitionPage::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter painter;
    painter.begin(this);

    // redraw the chart
    Nightcharts PieChart;

    // use a 3D look
    PieChart.setType(Nightcharts::Pie);

    // legend is on the side
    PieChart.setLegendType(Nightcharts::Round);

    // compute the new size
    PieChart.setCords(50, height()/2 - 150, 500, 500);

    // add the pieces
    foreach(Partition part, m_parts)
    {
        // take only the paritions that correspond to the currently selected drive
        if(!(part.device() == m_dev_box->currentText()))
            continue;

        // compute the percentage
        PedDevice *device;
        device = ped_device_get(m_dev_box->currentText().toStdString().c_str());
        float percentage = (float(part.size()) / device->length)*100;

        // if our partition is selected, highlight it :)
        QColor color = part.color();
        // first find the item that corresponds to it
        foreach(QTreeWidgetItem *it, m_parts_items)
        {
            if(it->text(0) == part.path()) // it's the one
            {
                // check if it's selected
                if(!m_current_item)
                    break;
                if(m_current_item == it) // add a highlight
                    color = color.lighter();
            }
        }


        PieChart.addPiece(part.path(), color, percentage);
    }

    // add the legend and draw the chart
    PieChart.draw(&painter);
    // TODO : fix the legend position
    // PieChart.drawLegend(&painter);
}

void PartitionPage::resizeEvent(QResizeEvent *)
{
    m_title->resize(width(), 50);
    m_title->move(0,0);
    m_text->resize(width(), height()/2 - 250);
    m_text->move(0,50);
    m_dev_box->move(275 - m_dev_box->width()/2, height()/2 - 200);
    m_tree->resize(width() - 700, 400);
    m_tree->move(width() - m_tree->width() - 50, height()/2 - 200);
    m_resize->resize(m_tree->width()/2 - 20, 50);
    m_delete->resize((m_tree->width()/2) - 20, 50);
    m_resize->move(m_tree->x() + 10, (m_tree->y()+m_tree->height()) + 10);
    m_delete->move(m_resize->x() + m_resize->width() + 20, m_tree->y()+m_tree->height() + 10);
    qDebug() << m_tree->pos() << m_resize->pos() << m_delete->pos();
}

void PartitionPage::m_update_parts_list()
{
    // first, list the devices
    PedDevice *device = NULL;
    ped_device_probe_all();

    // reset the combo box and the parts lists
    m_dev_box->clear();
    m_parts.clear();
    m_tree->clear();
    m_devices.clear();
    m_parts_items.clear();
    while((device = ped_device_get_next(device)))
    {
        // add it to the box
        m_dev_box->addItem(QIcon("/usr/share/nebulae/installer/drive-harddisk.png"), device->path);

        // remember the index
        m_devices.append(device->path);

        // lists its partitions
        qDebug() << device->path;
        PedDisk *disk;
        PedPartition *part;
        disk = ped_disk_new(device);
        qDebug() << "====================";
        qDebug() << "Found Drive " << device->path << ". Size " << device->length;

        // add it to the tree widget
        QTreeWidgetItem *it = new QTreeWidgetItem(m_tree, QStringList(QString(device->path)));
        it->setIcon(0, QIcon("/usr/share/nebulae/installer/drive-harddisk.png"));
        m_tree->addTopLevelItem(it);

        int color_index = 0;
        for(part = ped_disk_next_partition(disk, NULL); part; part = ped_disk_next_partition(disk, part))
        {
            QString type, path;
            QColor color;
            QIcon icon;
            if(part->num < 0) {
                qDebug() << "Found partition, ID " << part->num;
                qDebug() << ">> Partition size is " << part->geom.length;
                qDebug() << ">> Partition beings at " << part->geom.start;

                if(part->geom.start == 0 && part->geom.length == 63)
                    continue;   // this one is the partition table

                type = "Empty space";
                path = "Empty space";
                color = Qt::black;
                icon = QIcon("/usr/share/nebulae/installer/icons/transpa.png");
            }
            else {
                qDebug() << "Found partition, ID " << part->num;
                qDebug() << ">> Partition size is " << part->geom.length;
                qDebug() << ">> Partition beings at " << part->geom.start;

                if(!part->fs_type)
                    continue;
                qDebug() << ">> FS Type is " << type;

                type = QString(part->fs_type->name);
                path = QString(device->path) + QString::number(part->num);
                color = m_color(color_index);
                icon = m_icon(type);

                if(color_index == 5)
                    color_index = 0;
                else
                    color_index++;
            }

            // add it to the list
            Partition prt(path, device->path, part->geom.length, type, icon, color);
            m_parts.append(prt);
            prt.setStart(part->geom.start);
            prt.setEnd(part->geom.end);

            // show it in the tree
            QTreeWidgetItem *prt_it = new QTreeWidgetItem(it);
            prt_it->setText(0, path);
            double disp_size = double(prt.size())*512;     // size in octets
            QString unit = " o.";
            if(disp_size > 1000000000) {     // greater than 1 Go
                double go_size = disp_size / 1000000000;   // int divide return the quotient only
                disp_size = go_size;
                unit = " Go.";
            }
            else if(disp_size > 1000000) {      // Mo
                double mo_size = disp_size / 1000000;   // int divide return the quotient only
                disp_size = mo_size;
                unit = " Mo.";
            }
            else if(disp_size > 1000) {     // ... and ko
                double ko_size = disp_size / 1000;
                disp_size = ko_size;
               unit = " Ko.";
            }
            prt_it->setText(1, QString::number(disp_size, 'f', 2) + unit);
            prt_it->setIcon(0, icon);
            m_parts_items.append(prt_it);
        }
    }
    m_tree->expandAll();
}

void PartitionPage::m_device_changed(QString)
{
    // juste update the PieChart with the new device
    repaint();
}

void PartitionPage::m_tree_clicked(QTreeWidgetItem *it, QTreeWidgetItem *)
{
    // get the path of the clicked item
    QString path = it->text(0);
    m_current_item = it;

    // is it an empty partition ?
    if(path == "Empty space")
    {
        // get its device
        QTreeWidgetItem *dev = it->parent();
        path = dev->text(0);
    }

    // look in the list if  it's a partition
    foreach(Partition part, m_parts)
    {
        if(part.path() == path)
        {
            // it's the one ! :D
            // just display it's device
            m_dev_box->setCurrentIndex(m_devices.indexOf(part.device()));
            repaint();
            return;
        }
    }

    // we didn't find anything, it's a device, display it
    m_dev_box->setCurrentIndex(m_devices.indexOf(path));

    repaint();
}
