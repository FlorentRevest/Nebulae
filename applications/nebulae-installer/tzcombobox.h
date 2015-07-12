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

#ifndef TZCOMBOBOX_H
#define TZCOMBOBOX_H

#include <QtCore/QHash>

#include <QtGui/QWidget>
#include <QtGui/QComboBox>

class TZComboBox : public QWidget
{
    Q_OBJECT
public:
    explicit TZComboBox(QWidget *parent = 0);
    QString currentText();

signals:
    // emitted when the user changes the current item
    void currentIndexChanged(const QString &tz, const QString &town);

public slots:
    // changes the current index by indicating the text, such as "Europe" and "Paris"
    void setCurrentIndex(const QString &tz, const QString &town);

private slots:
    // called when the user change the current item of one of the boxes
    void m_tz_changed(QString);
    void m_town_changed(QString);

private:
    // the boxes
    QComboBox *m_tz_box, *m_town_box;

    // lists for indexing widgets
    QStringList m_tz_list, m_town_list, m_town_indexes;
    QHash<QString,QString> m_tz_towns;


};

#endif // TZCOMBOBOX_H
