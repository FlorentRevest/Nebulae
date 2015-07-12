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

#include "partition.h"

Partition::Partition(const QString &path, const QString &device, int size, const QString &fstype, const QIcon &icon, const QColor &color, const QString label) :
        m_path(path),
        m_device(device),
        m_size(size),
        m_label(label),
        m_icon(icon),
        m_color(color),
        m_fs_type(fstype)
{
}

void Partition::setPath(const QString &path)
{
    m_path = path;
}

QString Partition::path()
{
    return m_path;
}

void Partition::setDevice(const QString &device)
{
    m_device = device;
}

QString Partition::device()
{
    return m_device;
}

void Partition::setSize(int size)
{
    m_size = size;
}

int Partition::size()
{
    return m_size;
}


void Partition::setLabel(const QString &label)
{
    m_label = label;
}

QString Partition::label()
{
    return m_label;
}

void Partition::setIcon(const QIcon &icon)
{
    m_icon = icon;
}

QIcon Partition::icon()
{
    return m_icon;
}

void Partition::setColor(const QColor &color)
{
    m_color = color;
}

QColor Partition::color()
{
    return m_color;
}

void Partition::setType(const QString &type)
{
    m_fs_type = type;
}

QString Partition::type()
{
    return m_fs_type;
}

void Partition::setStart(int start)
{
    m_start = start;
}

int Partition::start()
{
    return m_start;
}

void Partition::setEnd(int end)
{
    m_end = end;
}

int Partition::end()
{
    return m_end;
}
