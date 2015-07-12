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

#include "include/musiccontrol.h"

MusicControl::MusicControl(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("QPushButton{ background-color: transparent; border: none; color:white; }"
                  "QPushButton:pressed{ background-color: qradialgradient(spread:pad, cx:0.468355, cy:0.472, radius:0.449, fx:0.472658, fy:0.477045, stop:0.261603 rgba(255, 255, 255, 130), stop:1 rgba(255, 255, 255, 0));}"
                  "QPushButton:checked{ background-color: qradialgradient(spread:pad, cx:0.468355, cy:0.472, radius:0.449, fx:0.472658, fy:0.477045, stop:0.261603 rgba(255, 255, 255, 130), stop:1 rgba(255, 255, 255, 0));}"
                  "QLabel{ color:white; }");

    m_c = Core::instance();
    connect(m_c, SIGNAL(lengthChanged()), this, SLOT(updateTime()));
    connect(m_c, SIGNAL(posChanged(int)), this, SLOT(updateTime()));
    connect(m_c, SIGNAL(tagsChanged()), this, SLOT(showTags()));
    connect(m_c, SIGNAL(currentMusicChanged()), this, SLOT(musicChanged()));
    connect(m_c, SIGNAL(videoAppearanceChanged(bool)), this, SLOT(moveWidgets()));
    connect(m_c, SIGNAL(isPlayingChanged()), this, SLOT(isPlayingChanged()));

    m_cover = new Cover(this);

    m_musicTagsLabel = new QLabel(this);
    m_musicTagsLabel->setAlignment(Qt::AlignCenter);
    m_musicTagsLabel->setText(QString(tr("<b>Welcome!</b>")));
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setOffset(1, 1);
    m_musicTagsLabel->setGraphicsEffect(shadowEffect);

//Player button, slider and label
    m_previousButton = new QPushButton(this);
    m_previousButton->setIcon(QIcon(QString(":/Player_Previous.png")));
    m_previousButton->setIconSize(QSize(32, 32));
    m_previousButton->setShortcut(QKeySequence(Qt::Key_Left));
    m_previousButton->setToolTip(tr("Previous media in the playlist"));
    connect(m_previousButton, SIGNAL(clicked()), this, SLOT(previous()));

    m_playButton = new QPushButton(this);
    m_playButton->setIcon(QIcon(QString(":/Player_Play.png")));
    m_playButton->setIconSize(QSize(50, 50));
    m_playButton->setShortcut(QKeySequence(Qt::Key_Space));
    m_playButton->setToolTip(tr("Play/Pause"));
    connect(m_playButton, SIGNAL(clicked()), this, SLOT(play()));

    m_nextButton = new QPushButton(this);
    m_nextButton->setIcon(QIcon(QString(":/Player_Next.png")));
    m_nextButton->setIconSize(QSize(32, 32));
    m_nextButton->setShortcut(QKeySequence(Qt::Key_Right));
    m_nextButton->setToolTip(tr("Next media in the playlist"));
    connect(m_nextButton, SIGNAL(clicked()), this, SLOT(next()));

    m_volumeIcon = new QPushButton(this);
    m_volumeIcon->setIcon(QIcon(":/Volume.png"));
    m_volumeIcon->setIconSize(m_volumeIcon->size());
    m_volumeIcon->setToolTip(tr("Mute or not"));
    connect(m_volumeIcon, SIGNAL(clicked()), this, SLOT(mute()));
    m_volumeIcon->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));

    m_volumeSlider = new QSlider(this);
    m_volumeSlider->setOrientation(Qt::Vertical);
    m_volumeSlider->setToolTip(tr("Change the volume"));
    m_volumeSlider->setValue(m_c->volume());
    m_volumeSlider->setMaximum(200);
    connect(m_volumeSlider, SIGNAL(valueChanged(int)), m_c, SLOT(setVolume(int)));

    m_seekSlider = new QSlider(this);
    m_seekSlider->setOrientation(Qt::Horizontal);
    m_seekSlider->setToolTip(tr("Change the position in the media"));
    connect(m_seekSlider, SIGNAL(sliderMoved(int)), m_c, SLOT(setPosition(int)));

    m_timeLabel = new QLabel(this);
    m_timeLabel->setMinimumSize(150, 0);
    m_timeLabel->setAlignment(Qt::AlignRight);
    m_timeLabel->setText("0:00 / 0:00");

    m_randButton = new QPushButton(this);
    m_randButton->setIconSize(QSize(22, 22));
    m_randButton->setIcon(QIcon(QString(":/Random.png")));
    m_randButton->setCheckable(true);
    m_randButton->setToolTip(tr("Random medias in the playlist or not"));
    m_randButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    m_repeatButton = new QPushButton(this);
    m_repeatButton->setIconSize(QSize(22, 22));
    m_repeatButton->setIcon(QIcon(QString(":/Repeat.png")));
    m_repeatButton->setCheckable(true);
    m_repeatButton->setToolTip(tr("Repeat the media or not"));
    m_repeatButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));

    moveWidgets();
}

void MusicControl::paintEvent(QPaintEvent *) // Draws the beautifull gradient !
{
    QPainter painter(this);
    QColor mainColor = m_c->mainColor();

    QLinearGradient grad1(0, 0, 0, height()/2);
    grad1.setColorAt(0, mainColor.lighter(110));
    grad1.setColorAt(1, mainColor.darker(130));
    painter.fillRect(0, 0, width(), height(), grad1);

    QLinearGradient grad2(0, height()/2, 0, height());
    grad2.setColorAt(0, mainColor.lighter(60));
    grad2.setColorAt(1, mainColor);
    painter.fillRect(0, height()/2, width(), height()/2, grad2);
}

void MusicControl::resizeEvent(QResizeEvent *)
{
    moveWidgets();
}

void MusicControl::moveWidgets() // Move the widgets to use all the space
{
    if(!m_c->hasVideo())
    {
        m_previousButton->setGeometry(width()/2 - 58, height() - 49, 32, 32);
        m_playButton->setGeometry(width()/2 - 22, height() - 60, 44, 44);
        m_nextButton->setGeometry(width()/2 + 23, height() - 49, 32, 32);
        m_seekSlider->setGeometry(3, height() - 22, width() - 6, 22);
        m_timeLabel->move(width() - 3 - m_timeLabel->width(), height() - 33);
        m_randButton->setGeometry(width() - 64, height() - 60, 28, 28);
        m_repeatButton->setGeometry(width() - 32, height() - 60, 28, 28);
        m_cover->show();
        m_cover->setGeometry(3, 3, height() - 5, height() - 25);
        m_volumeIcon->setGeometry(width() - 35, 3, 32, 32);
        m_volumeIcon->setIconSize(QSize(32, 32));
        m_volumeSlider->setOrientation(Qt::Vertical);
        m_volumeSlider->setGeometry(width() - 35, 35, 32, height() - (height() - m_repeatButton->y()) - 35);
        m_musicTagsLabel->show();
        m_musicTagsLabel->setGeometry(0, 0, width(), height() - (height() - m_playButton->y()));
        resize(parentWidget()->width(), 150);
        parentWidget()->resize(parentWidget()->size());
    }
    else
    {
        m_previousButton->setGeometry(2, height() - 32, 32, 32);
        m_playButton->setGeometry(34, height() - 44, 44, 44);
        m_nextButton->setGeometry(78, height() - 32, 32, 32);
        m_seekSlider->setGeometry(113, height() - 22, width() - 114, 22);
        m_volumeIcon->setGeometry(113, height() - 43, 22, 22);
        m_volumeIcon->setIconSize(QSize(22, 22));
        m_volumeSlider->setGeometry(140, height() - 43, 100, 22);
        m_volumeSlider->setOrientation(Qt::Horizontal);
        m_randButton->setGeometry(244, height() - 43, 22, 22);
        m_repeatButton->setGeometry(268, height() - 43, 22, 22);
        m_timeLabel->move(width() - 3 - m_timeLabel->width(), height() - 40);
        m_cover->hide();
        m_musicTagsLabel->hide();
        resize(parentWidget()->width(), 44);
        parentWidget()->resize(parentWidget()->size());
    }
}

void MusicControl::play() // Play or pause the current media
{
    if(m_c->currentPlaylist() != 0)
    {
        if(m_c->currentPlaylist()->currentIndex() == -1)
        {
            if(m_c->currentPlaylist()->isNull())
                QMessageBox::critical(0, tr("Any music can be played"), tr("Please select a music!"));
            else
                m_c->currentPlaylist()->setCurrentIndex(0);
        }
        if(!m_c->currentURL().isEmpty())  // If a music is selected
        {
            if(!m_c->isPlaying())
            {
                m_c->play();
                m_playButton->setIcon(QIcon(QString(":/Player_Pause.png")));
            }
            else
            {
                m_c->pause();
                m_playButton->setIcon(QIcon(QString(":/Player_Play.png")));
            }
        }
    }
}

void MusicControl::previous() // Change to the previous media in the current playlist
{
    bool mustPlay = m_c->isPlaying();
    m_c->previous();
    if(mustPlay)
        m_c->play();
    else
        m_c->pause();
}

void MusicControl::next() // Change to the next media in the current playlist
{
    bool mustPlay = m_c->isPlaying();
    m_c->next();
    if(mustPlay)
        m_c->play();
    else
        m_c->pause();
}

void MusicControl::mute()
{
    if(m_c->isMuted())
    {
        m_c->setMuted(false);
        m_volumeIcon->setIcon(QIcon(":/Volume.png"));
    }
    else
    {
        m_c->setMuted(true);
        m_volumeIcon->setIcon(QIcon(":/Volume_Mute.png"));
    }
}

void MusicControl::mousePressEvent(QMouseEvent *event) //Called when the user click on the MusicControl, works only if it's a video
{
    if(!window()->isMaximized() && m_c->hasVideo())
    {
        m_Diff = event->pos();
        setCursor(QCursor(Qt::SizeAllCursor));
    }
}

void MusicControl::mouseReleaseEvent(QMouseEvent *) //Called when the user release the mouse
{
    if(!window()->isMaximized() && m_c->hasVideo())
        setCursor(QCursor(Qt::ArrowCursor));
}

void MusicControl::mouseMoveEvent(QMouseEvent *event) //Called when the user move the mouse after click on the MusicControl
{
    if(!window()->isMaximized() && m_c->hasVideo())
        window()->move(event->globalPos() - m_Diff);
}

void MusicControl::musicChanged()
{
    if(!m_c->currentMusic() != 0)
        m_cover->search(m_c->currentMusic()->album());
}

void MusicControl::isPlayingChanged()
{
    if(m_c->isPlaying())
        m_playButton->setIcon(QIcon(QString(":/Player_Pause.png")));
    else
        m_playButton->setIcon(QIcon(QString(":/Player_Play.png")));
}

void MusicControl::showTags() //We have to use the Phonon's metadata resolver to recup a flux's metadata
{
    QMap <QString, QString> metaData = m_c->metaData();
    QString text;
    text.append("<b>");
    if(metaData.value("TITLE") != QString::Null())
    {
        text.append(metaData.value("TITLE"));
        parentWidget()->setWindowTitle(QString("Xound - " + metaData.value("TITLE")));
    }
    else
        parentWidget()->setWindowTitle(QString("Xound - What's a life whithout music?"));
    if(metaData.value("ARTIST") != QString::Null())
        text.append(QString(" - ") + metaData.value("ARTIST"));
    text.append("</b><br>");
    if(metaData.value("TRACKNUMBER") != QString::Null())
        text.append(tr("Track ") + metaData.value("TRACKNUMBER"));
    if(metaData.value("ALBUM") != QString::Null())
        text.append(tr(" of the album ") + metaData.value("ALBUM"));
    if(metaData.value("DATE") != QString::Null())
        text.append(tr(" in ") + metaData.value("DATE"));

    m_musicTagsLabel->setText(text);
}

void MusicControl::updateTime()
{
    long len = m_c->length();
    m_seekSlider->setMaximum(len);
    long pos = m_c->position();
    m_seekSlider->setValue(pos);
    QString timeString;
    if (pos || len)
    {
        int sec = pos/1000;
        int min = sec/60;
        int hour = min/60;
        int msec = pos;

        QTime playTime(hour%60, min%60, sec%60, msec%1000);
        sec = len / 1000;
        min = sec / 60;
        hour = min / 60;
        msec = len;

        QTime stopTime(hour%60, min%60, sec%60, msec%1000);
        QString timeFormat = "m:ss";

        if(hour > 0)
            timeFormat = "h:mm:ss";

        timeString = playTime.toString(timeFormat);

        if(len)
            timeString += " / " + stopTime.toString(timeFormat);
    }
    m_timeLabel->setText(timeString);
}

