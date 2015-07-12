#ifndef FREEDESKTOPSETTINGS_H
#define FREEDESKTOPSETTINGS_H

#include <QtCore/QSettings>

/**
  * \class FreedesktopSettings
  * \brief FreedesktopSettings is a surcouch of QSettings to read/write
  * all Freedesktop config files.
  * By default, QSettings has problems to read those files since they are
  * full UTF-8 encoded, and many values are separated by ; whereas QSettings
  * when reading stops after the first ;
  * This class use a custom QSettings::Format with it's own write methods to
  * solve this problem.
  * Those methods are accessible via freedestkopReadFunc and freedesktopWriteFunc
  * if you want to use them in your own programs.
  */
class FreedesktopSettings : public QSettings
{
    Q_OBJECT

public:
    /**
      * \brief Builds a FreedesktopSettings with the given file.
      * \param filePath Must be an absolute file path to a file.
      */
    explicit FreedesktopSettings(const QString &filePath, QObject *parent = 0);

    static bool freedesktopReadFunc (QIODevice &device, QSettings::SettingsMap &map);
    static bool freedestkopWriteFunc(QIODevice &device, const QSettings::SettingsMap &map);
};

#endif // FREEDESKTOPSETTINGS_H
