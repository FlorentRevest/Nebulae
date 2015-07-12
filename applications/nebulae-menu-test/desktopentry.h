#ifndef DESKTOPENTRY_H
#define DESKTOPENTRY_H

#include <QtCore/QFile>

class DesktopEntry : public QFile
{
public:
    explicit DesktopEntry(const QString &fileName, QObject *parent = 0);

    QString name();
    QString comment();
    QString icon();

private:
    QString m_locale;
    QString m_value(QString, bool);
};

#endif // DESKTOPENTRY_H
