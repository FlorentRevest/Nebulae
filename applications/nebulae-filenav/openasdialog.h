#ifndef OPENASDIALOG_H
#define OPENASDIALOG_H

#include <QDialog>
#include <QListWidget>

class OpenAsDialog : public QDialog
{
    Q_OBJECT

public:
    // The first parameter stands for the path of the file we will have to open. The second one is the
    // "mime type" of the file, like "image/gif" or "text/plain"
    OpenAsDialog(const QString &filePath, const QString &mimeType, QWidget *parent = 0);

private slots:
    // called when the user press ok, launch the selected application
    void m_launch();

private:
    // Internaly called when we found an application, to add the item in the view
    // get the path of the .desktop that launchs this application
    void m_add_application(const QString &application_desktop_file_path);

    // The file we have to open (get from the cosntructor's parameters
    QString m_file_path;

    // The display widget
    QListWidget *m_list;
};

#endif // OPENASDIALOG_H
