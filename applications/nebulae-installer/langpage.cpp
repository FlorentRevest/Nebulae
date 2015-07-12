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

#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QTranslator>

#include <QtGui/QLabel>
#include <QtGui/QPixmap>
#include <QtGui/QComboBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QApplication>

#include "langpage.h"
#include "infomanager.h"
#include "pictureflow.h"
#include "common.h"
#include "tzcombobox.h"

#include <QDebug>

LangPage::LangPage(InfoManager *inf, QWidget *parent) : QWidget(parent)
{
    // this page allows to set the lang of the OS in a particular way
    // the user choose a country, that defines the langage, the hour,
    // the jeyboard layout, and other options

    // initialize the class that will store the options, passed by the main window
    m_inf_mgr = inf;

    // the translator
    m_translator = new QTranslator(this);

    // the display uses a apple-like CoverFlow
    flow = new PictureFlow(this);
    flow->setSlideSize(QSize(450,300));

    // build other widgets and insert them in a grid layout below the coverflow
    m_country_label = new QLabel(tr(""), this);
    m_country_label->setAcceptDrops(Qt::AlignCenter);
    m_lang_label = new QLabel(tr("Langage defined to :"), this);
    m_keyboard_label = new QLabel(tr("Keyboard layout defined to: "), this);
    m_timezone_label = new QLabel(tr("Time zone defined to: "), this);
    m_lang_box = new QComboBox(this);
    m_keyboard_box = new QComboBox(this);
    m_timezone_box = new TZComboBox(this);
    QGridLayout *bottom_layout = new QGridLayout(this);
    bottom_layout->addWidget(m_country_label, 0, 0, 1, 2, Qt::AlignCenter);
    bottom_layout->addWidget(m_lang_label, 1, 0, 1, 1, Qt::AlignRight);
    bottom_layout->addWidget(m_lang_box, 1, 1, 1, 1, Qt::AlignLeft);
    bottom_layout->addWidget(m_keyboard_label, 2, 0, 1, 1, Qt::AlignRight);
    bottom_layout->addWidget(m_keyboard_box, 2, 1, 1, 1, Qt::AlignLeft);
    bottom_layout->addWidget(m_timezone_label, 3, 0, 1, 1, Qt::AlignRight);
    bottom_layout->addWidget(m_timezone_box, 3, 1, 1, 1, Qt::AlignLeft);
    m_bottom_widget = new QWidget(this);    // widget that'll contain everything
    m_bottom_widget->setLayout(bottom_layout);
    m_bottom_widget->resize(width(), 150);
    m_bottom_widget->setBackgroundRole(QPalette::Dark);
    m_bottom_widget->setStyleSheet(".QWidget { background-color: black; }");

    // populates the boxes and connect their change event
    // 1. the langage
    QSettings s(RESSOURCE_DIR + "locales.conf", QSettings::NativeFormat, this);
    s.beginGroup("Names");
    foreach(QString key, s.allKeys())
    {
        // get the full name that corresponds to the locale and cache it
        QString localename = QString::fromUtf8(s.value(key).toByteArray());
        m_locales.insert(key, localename);
        s.endGroup();

        // remember the index
        m_locales_indexes.append(key);

        // get the icon
        s.beginGroup("Flags");
        QString flagpath = RESSOURCE_DIR + "flags/" + s.value(key).toString();
        s.endGroup();
        s.beginGroup("Names");

        // add the item to the box
        m_lang_box->addItem(QIcon(flagpath), localename);
    }

    // 2. the keyboard
    // TODO: to be implemented

    connect(m_lang_box, SIGNAL(currentIndexChanged(QString)), this, SLOT(m_lang_changed(QString)));
    connect(m_timezone_box, SIGNAL(currentIndexChanged(QString,QString)), this, SLOT(m_timezone_changed(QString,QString)));

    // explore the configuration file
    QSettings conf("/usr/share/nebulae/installer/countries.conf", QSettings::NativeFormat, this);
    foreach(QString grp, conf.childGroups())
    {
        // just read the picture and associate the name of the country
        // to it's index in the flow
        conf.beginGroup(grp);

        // verify the picture is loadable
        QPixmap pix;
        qDebug() << "/usr/share/nebulae/installer/flags/" + conf.value("Flag", "unknown.png").toString();
        if(pix.load("/usr/share/nebulae/installer/flags/" + conf.value("Flag", "unknown.png").toString()))
        {
            flow->addSlide(pix);

            // the index is determined by the position in this list :
            m_countries.append(grp);
        }

        conf.endGroup();
    }

    // don't let the left side empty by default
    connect(flow, SIGNAL(centerIndexChanged(int)), this, SLOT(m_index_changed(int)));
    flow->setCenterIndex(1);
    m_index_changed(1);
    m_locale = m_locales.key(m_lang_box->currentText());
    m_timezone = m_timezone_box->currentText();
    qDebug() << m_timezone;
}

void LangPage::m_index_changed(int index)
{
    // get the group name of the country in the conf by using the index
    QString grp = m_countries.at(index);

    // read back the informations from the file
    QSettings conf("/usr/share/nebulae/installer/countries.conf", QSettings::NativeFormat, this);
    conf.beginGroup(grp);
    QString name = conf.value("Name").toString();
    QString locale = conf.value("Locale", "en_US").toString();
    QString kblayout = conf.value("Keyboard").toString();
    QString timezone = conf.value("Timezone").toString();
    conf.endGroup();

    // load the translations to the ui
    if(m_translator->load("translation." + locale, TRANSLATIONS, QString(), ".qm"))
    {
        qApp->installTranslator(m_translator);
    }

    // change the combo box (and the configuration by the way)
    m_lang_box->setCurrentIndex(m_locales_indexes.indexOf(locale));
    QStringList prts = timezone.split('/');
    m_timezone_box->setCurrentIndex(prts.first(), prts.last());
    m_locale = locale;
    m_timezone = timezone;
    qDebug() << timezone;

    // finally, display the name of the country
    m_country_label->setText("<strong>" + name + "</strong>");
}

void LangPage::resizeEvent(QResizeEvent *)
{
    flow->resize(width(), height() - m_bottom_widget->height());
    flow->move(0,0);
    m_bottom_widget->resize(width(), m_bottom_widget->height());
    m_bottom_widget->move(0, flow->height());
}

// TODO: fix it. It does not work at all !
bool LangPage::event(QEvent *ev)
{
    if(ev->type() == QEvent::LanguageChange)
    {
        // just update the text of the labels to use the new langage
        m_lang_label->setText(tr("Langage defined to :"));
        m_keyboard_label->setText(tr("Keyboard layout defined to: "));
        m_timezone_label->setText(tr("Time zone defined to: "));
    }
    else
        QWidget::event(ev);
}

void LangPage::m_lang_changed(QString localename)
{
    m_locale = m_locales.key(localename);
}

void LangPage::m_timezone_changed(QString tz, QString town)
{
    m_timezone = tz + "/" + town;
    qDebug() << m_timezone;
}

void LangPage::writeConfig()
{
    // just copy our options to the info manager
    m_inf_mgr->setLocale(m_locale);
    m_inf_mgr->setTimezone(m_timezone);
}
