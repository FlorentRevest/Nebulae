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

#ifndef PARTITION_H
#define PARTITION_H

#include <QtCore/QObject>

#include <QtGui/QIcon>
#include <QtGui/QColor>

class Partition
{
public:
    explicit Partition(const QString &path, const QString &device, int size, const QString &fstype, const QIcon &icon, const QColor &color, const QString label = QString());

    // acessors for the properties described below
    void setPath(const QString&);
    void setDevice(const QString&);
    void setSize(int);
    void setLabel(const QString&);
    void setIcon(const QIcon&);
    void setColor(const QColor&);
    void setType(const QString&);
    void setStart(int);
    void setEnd(int);

    QString path();
    QString device();
    int size();
    QString label();
    QIcon icon();
    QColor color();
    QString type();
    int start();
    int end();

private:
    // the path of the partition, like /dev/sda1
    QString m_path;

    // the path of the device the partition is contained in, like /dev/sda
    QString m_device;

    // the size of the partition
    int m_size;

    // its name, or label
    QString m_label;

    // the icon used to display it
    QIcon m_icon;

    // the color that represent it in the chart
    QColor m_color;

    // the fs type, such as "ext4" or "ntfs"
    QString m_fs_type;

    // the first and last sectors
    int m_start;
    int m_end;
};

#endif // PARTITION_H
