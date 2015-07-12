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

#include "searchbar.h"

SearchBar::SearchBar(QWidget *parent) : QWidget(parent) // This bar is part of the menu, it allows to search in the menu
{
    hover = false;
    m_searchLine = new QLineEdit(this);
    m_searchLine->setPlaceholderText("Search...");
    connect(m_searchLine, SIGNAL(textChanged(QString)), this, SLOT(search(QString)));
    m_searchLine->setStyleSheet("QLineEdit { border: 2px solid #333;"
                                "    border-radius: 15px;"
                                "    color: white;"
                                "    padding: 0 8px;"
                                "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(5, 5, 5, 116), stop:0.996094 rgba(89, 89, 89, 83));}");
}

void SearchBar::search(QString) // Called when the user changes the text of the lineEdit
{
    if(m_searchLine->text() == "")
        hover = false;
    else
        hover = true;
    update();
}

void SearchBar::paintEvent(QPaintEvent *) // When the user search something, shows a white background
{
    if(hover)
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(Qt::white);
        p.drawRoundedRect(0, -20, width(), height() + 20, 10, 10);
    }
}

void SearchBar::resizeEvent(QResizeEvent *) // The lineEdit has to be as big as the widget
{
    m_searchLine->setGeometry(2, 10, width() - 4, 30);
}
