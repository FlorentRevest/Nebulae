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

#ifndef PARTITIONPAGE_H
#define PARTITIONPAGE_H

#include <QtGui/QWidget>
#include "partition.h"

class PieChart;
class InfoManager;
class QLabel;
class QComboBox;
class QPushButton;
class QTreeWidget;
class QTreeWidgetItem;
class PartitionPage : public QWidget
{
    Q_OBJECT
public:
    explicit PartitionPage(InfoManager *inf, QWidget *parent = 0);

signals:

private slots:
    void m_device_changed(QString);
    void m_tree_clicked(QTreeWidgetItem*,QTreeWidgetItem*);

private:
    // the main display
    PieChart *m_chart;
    QTreeWidget *m_tree;
    QLabel *m_title, *m_text;

    // the action buttons
    QPushButton *m_resize, *m_delete;

    // the info manager
    InfoManager *m_inf_mgr;

    // list of devices
    QComboBox *m_dev_box;
    QList<Partition> m_parts;
    QList<QTreeWidgetItem*> m_parts_items;
    QTreeWidgetItem *m_current_item;
    QStringList m_devices;

    void m_update_parts_list();

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);

};

#endif // PARTITIONPAGE_H
