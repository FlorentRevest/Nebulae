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

#include "programdefaultseditor.h"
#include "nebulae-desktopentry.h"
#include "nebulae-mimetypeinfo.h"

#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QTreeWidget>
#include <QtGui/QPushButton>
#include <QtGui/QBoxLayout>

#include <QtDebug>

ProgramDefaultsEditor::ProgramDefaultsEditor(DesktopEntry *program, QWidget *parent) : QWidget(parent)
{
    // initialize widgets
    m_title_label = new QLabel(this);
    m_subtitle_label = new QLabel(this);
    m_name_label = new QLabel(this);
    m_icon_label = new QLabel(this);
    m_select_all = new QCheckBox(this);
    m_list = new QTreeWidget(this);

    QPushButton *save_button = new QPushButton(tr("Save"), this);
    QPushButton *cancel_button = new QPushButton(tr("Cancel"), this);

    // place items in a Layout
    QBoxLayout *main_layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(main_layout);
    main_layout->addWidget(m_title_label);
    main_layout->addWidget(m_subtitle_label);
    QWidget *info_widget = new QWidget(this);
    QBoxLayout *info_layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    info_widget->setLayout(info_layout);
    info_layout->addWidget(m_icon_label);
    info_layout->addWidget(m_name_label);
    info_layout->addStretch();
    main_layout->addWidget(info_widget);
    main_layout->addWidget(m_select_all);
    main_layout->addWidget(m_list);
    QWidget *bttn_widget = new QWidget(this);
    QBoxLayout *bttn_layout = new QBoxLayout(QBoxLayout::RightToLeft, this);
    bttn_widget->setLayout(bttn_layout);
    bttn_layout->addWidget(cancel_button);
    bttn_layout->addWidget(save_button);
    bttn_layout->addStretch();
    main_layout->addWidget(bttn_widget);

    // populate all informations
    m_title_label->setText("<strong>" + tr("Set associations for a program") + "</strong>");
    m_subtitle_label->setText("Select the types this program must open by default then press Save");
    m_select_all->setText(tr("Select all"));
    m_list->setHeaderLabels(QStringList() << tr("Name") << tr("Description") << tr("Actual default program"));
    m_list->setAllColumnsShowFocus(true);

    if(program)
    {
        QString name = program->name(QLocale::system().name()).isEmpty() ? program->name() : program->name(QLocale::system().name());
        m_name_label->setText(name);
        m_icon_label->setPixmap(program->icon().pixmap(32,32));
        foreach(QString type, program->mimeTypes())
        {
            QTreeWidgetItem *it = new QTreeWidgetItem(m_list);
            it->setText(0, type);
            it->setText(1, QString());
            MimeType m = nApp->mimeTypeDatabase()->findType(type);
            it->setIcon(0, QIcon::fromTheme(m.icon()));
            it->setText(1, nApp->mimeTypeDatabase()->description(m, QLocale::system().name().split('_').first()));
            DesktopEntry d = nApp->mimeTypeDatabase()->programForType(m);
            it->setText(2, d.name(QLocale::system().name()));
            it->setIcon(2, d.icon());
            it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
            it->setCheckState(0, Qt::Unchecked);
            if(d.filePath() == program->filePath())
                it->setCheckState(0, Qt::Checked);
        }
    }

    connect(m_select_all, SIGNAL(toggled(bool)), this, SLOT(m_select_all_toggled(bool)));
    connect(save_button, SIGNAL(clicked()), this, SLOT(m_save_button_pressed()));
    connect(cancel_button, SIGNAL(clicked()), this, SLOT(m_cancel_button_pressed()));

}

void setDefaultProgram(const QString &type)
{
    // write new user settings
    QFile f(XDG_DATA_HOME + "/applications/mimeapps.list");
    if(f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while(!f.atEnd())
        {
            QString line = QString::fromUtf8(f.readLine());
            if(line.startsWith('#'))
                continue;
            if(line.endsWith('\n'))
                line.truncate(line.length()-1);
        }
    }

}

void ProgramDefaultsEditor::m_save_button_pressed()
{
    QStringList lst;
    foreach(QTreeWidgetItem *it, m_list->findItems("", Qt::MatchStartsWith))
    {
        if(it->checkState(0) == Qt::Checked)
            lst << it->text(0);
    }
    qDebug() << lst;

    emit finished();
}

void ProgramDefaultsEditor::m_cancel_button_pressed()
{
    emit finished();
}

void ProgramDefaultsEditor::m_select_all_toggled(bool select)
{
    if(select)
    {
        foreach(QTreeWidgetItem *it, m_list->findItems("", Qt::MatchStartsWith)) // should return all items
            it->setCheckState(0,Qt::Checked);
    }
    else
    {
        foreach(QTreeWidgetItem *it, m_list->findItems("", Qt::MatchStartsWith))
            it->setCheckState(0,Qt::Unchecked);
    }
}
