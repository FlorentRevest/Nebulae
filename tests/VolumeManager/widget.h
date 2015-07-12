#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSlider>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);

private:
    QSlider *volSlider;
    int mixerfd;

private slots:
    void setVolume(int);
};

#endif // WIDGET_H
