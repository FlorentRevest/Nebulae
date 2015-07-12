#ifndef _IMAGE_VIEWER_H_
#define _IMAGE_VIEWER_H_

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QPixmap>
#include <QtGui/QScrollArea>

class QBoxLayout;
class ImageViewer : public QWidget
{
    Q_OBJECT

    // Enum structure that destcribes the two possible display ways
    enum ViewMode {
        Adjust,         // the picture will be adapted to the window side
        Fixed           // the picture is zoomed at a fixed size
    };

public:
    ImageViewer(QWidget *parent = 0);
    ~ImageViewer();

public slots:
    void setPicture(const QString &path);
    void zoomIn();
    void zoomOut();
    void adjust();

private:
    // updates the view
    void m_display();

    // display widgets
    QLabel *m_label, *m_area_label;
    QScrollArea *m_area;
    QBoxLayout *m_layout;

    // the path of the picture
    QString m_path;

    // the zoom factor, only used in fixed mode
    int m_zoom_factor;

    // determines wich ViewMode is used
    ViewMode m_mode;

protected:
    virtual void resizeEvent(QResizeEvent *);
};

#endif
