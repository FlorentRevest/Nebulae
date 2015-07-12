#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QHash>

#include <QtGui/QMainWindow>
#include <QtGui/QListWidget>
#include <QtGui/QLabel>

class ImageViewer;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString dir_path, QString file_name = 0, QWidget *parent = 0);

private slots:
    // called when the user click on an image at the bottom
    void m_image_clicked(QListWidgetItem*);

private:
    // the three main widgets of the window
    ImageViewer *m_viewer;   // the main display
    QWidget *m_toolbar;      // the toolbar
    QListWidget *m_list;     // the list of other pictures

    // used by the list to associate a button to a file
    QHash<QListWidgetItem*,QString> m_list_hash;

    // a label used to display the current dir
    QLabel *m_dir_label;

    // method that populates the tool bar
    void *m_build_toolbar();

protected:
    virtual void resizeEvent(QResizeEvent *);
};

#endif // MAINWINDOW_H
