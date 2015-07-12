#ifndef NEBULAEAPPLICATION_H
#define NEBULAEAPPLICATION_H

#include <QtGui/QApplication>

#include "nebulae-mimetypeinfo.h"

#if defined(nApp)
#undef nApp
#endif
#define nApp (static_cast<NebulaeApplication*>(qApp))

/**
  * \class NebulaeApplication
  *
  * The NebulaeApplication class represents a GUI application.
  *
  * Based on Qt's QApplication, it adds native support of some
  * tasks that are atomatically done, such as translation loading,
  * settings files creation... etc.
  */
class MimeTypeDatabase;
class NebulaeApplication : public QApplication
{

    Q_OBJECT

public:

    /**
      * \brief Initialize the window system and the application instance
      * \param argc Number of command-line arguments, juste like the argc parameter of the C main() function
      * \param argv Array of caracters strings taht contains every commnd-line arguments
      * \param name The machine-friendly name of the application, that will be used to locate settings files
      */
    NebulaeApplication(int argc, char **argv, const QString &name);

    MimeTypeDatabase *mimeTypeDatabase() { return m_db; }

private:
    MimeTypeDatabase *m_db;
};

#endif // NEBULAEAPPLICATION_H
