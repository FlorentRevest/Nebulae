#include "widget.h"

RadioPlugin::RadioPlugin()
{
    QTranslator *translation = new QTranslator();
    translation->load(QString("Radio_") + QLocale::system().name().section('_', 0, 0));
    qApp->installTranslator(translation);
}

QString RadioPlugin::name() const
{
    return tr("Radio").toAscii();
}

QIcon RadioPlugin::icon() const
{
    return QIcon(":/radio.png");
}

void RadioPlugin::setMusicPath(QString currentMusicPath)
{}

Q_EXPORT_PLUGIN2(RadioPlugin, RadioPlugin)
