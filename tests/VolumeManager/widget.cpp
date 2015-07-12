#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    char *mixer_dev = "/dev/mixer";
    mixerfd = 0;
    int devmask = 0;

    if((mixerfd = open(mixer_dev, O_RDWR)) < 0)
      qDebug(tr("Cant open mixer device").toAscii());


    if(ioctl(mixerfd, SOUND_MIXER_READ_DEVMASK, &devmask) == -1)
        qDebug(tr("Cant open mixer device for reading").toAscii());

    volSlider = new QSlider(this);
    connect(volSlider, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));


    int currentVol = 50;

    if (ioctl(mixerfd, SOUND_MIXER_READ_VOLUME, &currentVol) == -1)
        qDebug(tr("Cant open mixer device for writing").toAscii());
    else
      currentVol = (currentVol & 0xff) + ((currentVol >> 8) & 0xff) / 2;

    volSlider->setValue(currentVol);
}

void Widget::setVolume(int value)
{
    int new_vol = value | (value << 8);

    if (ioctl(mixerfd, SOUND_MIXER_WRITE_VOLUME, &new_vol)  == -1)
        qDebug(tr("Unable to set volume").toAscii());
}
