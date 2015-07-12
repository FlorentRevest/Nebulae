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

#include "widget.h"

WikiPlugin::WikiPlugin()
{
    m_c = Core::instance();
    connect(m_c, SIGNAL(currentMusicChanged()), this, SLOT(updatePages()));

    QGridLayout *layout = new QGridLayout();
    layout->setMargin(0);

    m_tabWidget = new QTabWidget(this);
    layout->addWidget(m_tabWidget);

    m_artistWebView = new QWebView();
    m_tabWidget->addTab(m_artistWebView, QIcon(":/wiki_artist.png"), "");
    m_albumWebView = new QWebView();
    m_tabWidget->addTab(m_albumWebView, QIcon(":/wiki_album.png"), "");
    m_titleWebView = new QWebView();
    m_tabWidget->addTab(m_titleWebView, QIcon(":/wiki_music.png"), "");

    setLayout(layout);
}

QString WikiPlugin::name() const
{
    return QString("Wikipedia");
}

QIcon WikiPlugin::icon() const
{
    return QIcon(":/wikipedia.png");
}

void WikiPlugin::updatePages()
{
    QString basePage = QString("http://") + tr("en") + ".m.wikipedia.org/wiki/";
    m_artistWebView->load(basePage + m_c->currentMusic()->artist()->name().replace(" ", "_"));
    m_albumWebView->load(basePage + m_c->currentMusic()->album()->name().replace(" ", "_"));
    m_titleWebView->load(basePage + m_c->currentMusic()->title().replace(" ", "_"));
}

Q_EXPORT_PLUGIN2(WikipediaPlugin, WikiPlugin)
