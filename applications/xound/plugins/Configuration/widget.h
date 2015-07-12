#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>
#include "../../include/contentplugin.h"

class ConfigurationPlugin : public ContentPlugin
{
    Q_OBJECT
    Q_INTERFACES(ContentPlugin)

public:
    ConfigurationPlugin();
    virtual QString name() const;
    virtual QIcon icon() const;
    virtual void setMusicPath(QString);
};

#endif // WIDGET_H
