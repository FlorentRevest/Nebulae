#include "widget.h"

ConfigurationPlugin::ConfigurationPlugin()
{
    QTranslator *translation = new QTranslator();
    translation->load(QString("Configuration_") + QLocale::system().name().section('_', 0, 0));
    qApp->installTranslator(translation);
}

QString ConfigurationPlugin::name() const
{
    return tr("Configuration").toAscii();
}

QIcon ConfigurationPlugin::icon() const
{
    return QIcon(":/configuration.png");
}

void ConfigurationPlugin::setMusicPath(QString currentMusicPath)
{}

Q_EXPORT_PLUGIN2(ConfigurationPlugin, ConfigurationPlugin)
