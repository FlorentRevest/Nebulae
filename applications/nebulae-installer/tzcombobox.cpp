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

#include "tzcombobox.h"

#include <QtCore/QFile>

#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>

TZComboBox::TZComboBox(QWidget *parent) : QWidget(parent)
{
    // initialize widgets and display
    m_tz_box = new QComboBox(this);
    m_town_box = new QComboBox(this);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_tz_box, 0, Qt::AlignRight);
    layout->addWidget(m_town_box, 0, Qt::AlignLeft);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    // populates the lists and the boxes
    QFile f("/usr/share/nebulae/installer/timezones");
    if(f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while(!f.atEnd())
        {
            // read one line from the file
            QString line = f.readLine();
            line.truncate(line.length()-1); // remove the \n

            // the line is type Europe/Paris
            QString tz = line.split('/').first();
            QString town = line.split('/').last();

            // if this is the first time we see this zone
            if(!m_tz_list.contains(tz))
            {
                // add the tz to the box
                m_tz_box->addItem(tz);

                // remember its index
                m_tz_list.append(tz);
            }

            // associate the town with it's timezone
            m_tz_towns.insert(town, tz);
            m_town_list.append(town);
        }
    }

    // populate the second box
    m_town_indexes.clear();
    foreach(QString town, m_town_list)
    {
        // if it corresponds to the zone, add it
        if(m_tz_towns.value(town) == m_tz_box->currentText())
        {
            // add it to the box
            m_town_box->addItem(town);

            // remember its index
            m_town_indexes.append(town);
        }
    }

    // connect signal/slots
    connect(m_tz_box, SIGNAL(currentIndexChanged(QString)), this, SLOT(m_tz_changed(QString)));
    connect(m_town_box, SIGNAL(currentIndexChanged(QString)), this, SLOT(m_town_changed(QString)));
}

void TZComboBox::m_tz_changed(QString)
{
    // update the list of the towns in the other box
    m_town_indexes.clear();
    m_town_box->clear();
    foreach(QString town, m_town_list)
    {
        // if it corresponds to the zone, add it
        if(m_tz_towns.value(town) == m_tz_box->currentText())
        {
            // add it to the box
            m_town_box->addItem(town);

            // remember its index
            m_town_indexes.append(town);
        }
    }
    m_town_box->setCurrentIndex(0);

    // tell the parent the tz have changed
    emit currentIndexChanged(m_tz_box->currentText(), m_town_box->currentText());
}

void TZComboBox::m_town_changed(QString)
{
    // tell the parent the tz have changed
    emit currentIndexChanged(m_tz_box->currentText(), m_town_box->currentText());
}

void TZComboBox::setCurrentIndex(const QString &tz, const QString &town)
{
    // use the index list for changing box values
    m_tz_box->setCurrentIndex(m_tz_list.indexOf(tz));
    if(m_tz_towns.value(town) == tz)
        m_town_box->setCurrentIndex(m_town_indexes.indexOf(town));
}

// accessor for the full timezone (tz+town)
QString TZComboBox::currentText()
{
    return m_tz_box->currentText() + '/' + m_town_box->currentText();
}
