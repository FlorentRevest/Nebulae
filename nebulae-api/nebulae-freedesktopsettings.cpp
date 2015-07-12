#include "nebulae-freedesktopsettings.h"

#include <QtCore/QStringList>
#include <QtDebug>

bool read_(QIODevice &device, QSettings::SettingsMap &map)
{
    QString prefix;
    while(!device.atEnd())
    {
        QString line = QString::fromUtf8(device.readLine());
        if(line.endsWith('\n'))
            line.truncate(line.length()-1);

        if(line.isEmpty())
            continue;

        // remove trailing spaces
        while(line.endsWith(' '))
            line.truncate(line.length()-1);

        // it's a group
        if(line.startsWith('[') && line.endsWith(']'))
        {
            prefix = line.mid(1, line.length()-2);
            continue;
        }

        // it's a key
        QString key = line.split('=').first();
        QString val = line.mid(key.length()+1);
        if(!prefix.isEmpty())
            key.prepend(prefix + "/");
        map.insert(key,QVariant(val));
    }
    device.close();
}

bool write_(QIODevice &device, const QSettings::SettingsMap &map)
{
    QString currentPrefix;
    foreach(QString key, map.keys())
    {
        QString value = map.value(key).toString();

        if(!key.contains("/"))
            key.prepend("General/");

        QString prefix = key.split('/').first();
        key = key.mid(prefix.length()+1);

        if(prefix != currentPrefix)
        {
            device.write("[", 1);
            device.write(prefix.toUtf8(), prefix.length());
            device.write("]", 1);
            device.write("\n",1);
            currentPrefix = prefix;
        }

        QString write = key + "=" + value;
        device.write(write.toUtf8(), write.length());
        device.write("\n",1);
    }
}

FreedesktopSettings::FreedesktopSettings(const QString &filePath, QObject *parent) : QSettings(filePath, registerFormat("Freedesktop", read_, write_), parent)
{
}

bool FreedesktopSettings::freedesktopReadFunc(QIODevice &device, QSettings::SettingsMap &map)
{
    return read_(device, map);
}

bool FreedesktopSettings::freedestkopWriteFunc(QIODevice &device, const QSettings::SettingsMap &map)
{
    return write_(device, map);
}
