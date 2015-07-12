#include "desktopentry.h"

#include <QtCore/QLocale>
#include <QtCore/QStringList>

DesktopEntry::DesktopEntry(const QString &fileName, QObject *parent) :
    QFile(fileName, parent)
{
    open(QIODevice::ReadOnly | QIODevice::Text);

    // FIXIT: get it in a more std way, considering encodings, variants, etc.
    m_locale = QLocale::system().name().section('_', 0, 0);
}

// TODO: allow to get more values, list values, and values in another locale
QString DesktopEntry::name()
{
    return m_value("Name", true);
}

QString DesktopEntry::comment()
{
    return m_value("Comment", true);
}

QString DesktopEntry::icon()
{
    return m_value("Icon", false);
}

QString DesktopEntry::m_value(QString value, bool use_locale)
{
    // reset and begin group
    seek(0);
    while(!atEnd())
    {
        if(readLine().trimmed() != "[DesktopEntry]")
            break;
    }

    // this is not a valud desktop entry !
    if(atEnd())
        return QString();

    // find the value
    QString ret;
    value = use_locale ? value + "[" + m_locale + "]" : value;
    while(!atEnd())
    {
        QString line = readLine().trimmed();
        if(line.split('=').at(0) == value)
            ret = line.mid(value.length() + 1);
    }
    return ret;
}
