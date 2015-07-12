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

#include "include/video.h"

Video::Video(QWidget *parent) : Phonon::VideoWidget(parent) // This Widget represents a simple video
{
    m_mc = new MusicControl(this);
    m_mc->hide();

    m_parent = parent;

    m_fullShown = false;

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

    settingsWindow = new QDialog();

    settingsWindow->resize(311, 271);
    verticalLayout_2 = new QVBoxLayout(settingsWindow);
    groupBox = new QGroupBox(settingsWindow);
    groupBox->setFlat(true);
    gridLayout = new QGridLayout(groupBox);
    label_9 = new QLabel(groupBox);
    gridLayout->addWidget(label_9, 0, 0, 1, 1);

    contrastSlider = new QSlider(groupBox);
    contrastSlider->setMinimum(-8);
    contrastSlider->setMaximum(8);
    contrastSlider->setOrientation(Qt::Horizontal);
    contrastSlider->setTickPosition(QSlider::TicksBelow);
    contrastSlider->setTickInterval(4);
    gridLayout->addWidget(contrastSlider, 0, 1, 1, 2);

    label_8 = new QLabel(groupBox);
    gridLayout->addWidget(label_8, 1, 0, 1, 1);

    brightnessSlider = new QSlider(groupBox);
    brightnessSlider->setMinimum(-8);
    brightnessSlider->setMaximum(8);
    brightnessSlider->setOrientation(Qt::Horizontal);
    brightnessSlider->setTickPosition(QSlider::TicksBelow);
    brightnessSlider->setTickInterval(4);
    gridLayout->addWidget(brightnessSlider, 1, 1, 1, 2);

    label_7 = new QLabel(groupBox);
    gridLayout->addWidget(label_7, 2, 0, 1, 1);

    saturationSlider = new QSlider(groupBox);
    saturationSlider->setMinimum(-8);
    saturationSlider->setMaximum(8);
    saturationSlider->setOrientation(Qt::Horizontal);
    saturationSlider->setTickPosition(QSlider::TicksBelow);
    saturationSlider->setTickInterval(4);
    gridLayout->addWidget(saturationSlider, 2, 1, 1, 2);

    label_2 = new QLabel(groupBox);
    gridLayout->addWidget(label_2, 3, 0, 1, 1);

    hueSlider = new QSlider(groupBox);
    hueSlider->setMinimum(-8);
    hueSlider->setMaximum(8);
    hueSlider->setOrientation(Qt::Horizontal);
    hueSlider->setTickPosition(QSlider::TicksBelow);
    hueSlider->setTickInterval(4);
    gridLayout->addWidget(hueSlider, 3, 1, 1, 2);

    label_10 = new QLabel(groupBox);
    gridLayout->addWidget(label_10, 4, 0, 1, 2);

    aspectCombo = new QComboBox(groupBox);
    aspectCombo->setMinimumSize(QSize(180, 0));
    gridLayout->addWidget(aspectCombo, 4, 2, 1, 1);
    label_11 = new QLabel(groupBox);
    gridLayout->addWidget(label_11, 5, 0, 1, 2);

    scalemodeCombo = new QComboBox(groupBox);
    scalemodeCombo->setMinimumSize(QSize(180, 0));
    gridLayout->addWidget(scalemodeCombo, 5, 2, 1, 1);
    verticalLayout_2->addWidget(groupBox);

    buttonBox = new QDialogButtonBox(settingsWindow);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    verticalLayout_2->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), settingsWindow, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), settingsWindow, SLOT(reject()));

    groupBox->setTitle(tr("Video options:"));
    label_9->setText(tr("Contrast:"));
    label_8->setText(tr("Brightness:"));
    label_7->setText(tr("Saturation:"));
    label_2->setText(tr("Hue:"));
    label_10->setText(tr("Aspect ratio:"));
    aspectCombo->clear();
    aspectCombo->insertItems(0, QStringList() << tr("Auto") << tr("Stretch") << tr("4/3") << tr("16/9"));
    label_11->setText(tr("Scale Mode:"));
    scalemodeCombo->clear();
    setCursor(Qt::ArrowCursor);
    scalemodeCombo->insertItems(0, QStringList() << tr("Fit in view") << tr("Scale and crop"));

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showMenu(QPoint)));

    brightnessSlider->setValue(int(brightness() * 8));
    hueSlider->setValue(int(hue() * 8));
    saturationSlider->setValue(int(saturation() * 8));
    contrastSlider->setValue(int(contrast() * 8));
    aspectCombo->setCurrentIndex(aspectRatio());
    scalemodeCombo->setCurrentIndex(scaleMode());

    connect(saturationSlider, SIGNAL(valueChanged(int)), this, SLOT(setSaturationValue(int)));
    connect(contrastSlider , SIGNAL(valueChanged(int)), this, SLOT(setContrastValue(int)));
    connect(brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(setBrightnessValue(int)));
    connect(hueSlider, SIGNAL(valueChanged(int)), this, SLOT(setHueValue(int)));
    connect(aspectCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setAspectValue(int)));
    connect(scalemodeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setScaleValue(int)));
}

void Video::mouseDoubleClickEvent(QMouseEvent *e) // Shows in full screen if the user double clicks
{
    Phonon::VideoWidget::mouseDoubleClickEvent(e);
    fullScreen();
}

void Video::resizeEvent(QResizeEvent *)
{
    m_mc->setGeometry(20 , height() - m_mc->height(), width() - 40, m_mc->height());
}

void Video::mouseMoveEvent(QMouseEvent *) // If the mouse is moved, shows the cursor
{
    m_timer->start(2500);
    setCursor(Qt::ArrowCursor);
}

void Video::timeout() // Hides the cursor
{
    m_timer->stop();
    setCursor(Qt::BlankCursor);
}

void Video::fullScreen() // Shows in full screen
{
    if(m_fullShown)
    {
        m_mc->hide();
        setParent(m_parent);
        setGeometry(m_geometry);
        show();
        m_fullShown = false;
    }
    else
    {
        m_mc->show();
        m_geometry = geometry();
        setParent(0);
        showFullScreen();
        m_fullShown = true;
    }
}

void Video::showMenu(const QPoint &) // Shows a little menu, only for the configuration of the video
{
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme("applications-system"), tr("Configuration"), this, SLOT(showConfWindow()));
    menu.exec(QCursor::pos());
}

void Video::showConfWindow() // This method shows the windows where you can change the video settings
{
    float oldBrightness = brightness();
    float oldHue = hue();
    float oldSaturation = saturation();
    float oldContrast = contrast();
    Phonon::VideoWidget::AspectRatio oldAspect = aspectRatio();
    Phonon::VideoWidget::ScaleMode oldScale = scaleMode();
    settingsWindow->exec();

    if (settingsWindow->result() == QDialog::Rejected)
    {
        setBrightness(oldBrightness);
        setSaturation(oldSaturation);
        setHue(oldHue);
        setContrast(oldContrast);
        setAspectRatio(oldAspect);
        setScaleMode(oldScale);
    }
}

void Video::setSaturationValue(int val)
    { setSaturation(val / 8); }

void Video::setHueValue(int val)
    { setHue(val / 8); }

void Video::setAspectValue(int val)
    { setAspectRatio(Phonon::VideoWidget::AspectRatio(val)); }

void Video::setScaleValue(int val)
    { setScaleMode(Phonon::VideoWidget::ScaleMode(val)); }

void Video::setBrightnessValue(int val)
    { setBrightness(val / 8); }

void Video::setContrastValue(int val)
    { setContrast(val / 8); }
