#include "imageviewer.h"

#include <QPixmap>
#include <QVBoxLayout>
#include <QPushButton>

ImageViewer::ImageViewer(QWidget *parent) : QWidget(parent)
{
    m_label = new QLabel();
    m_label->setAlignment(Qt::AlignCenter);
    m_area = new QScrollArea();
    m_area_label = new QLabel();
    m_mode = Adjust;
    m_zoom_factor = 100;

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0,0,0,0);
    setLayout(m_layout);

    m_display();
}

ImageViewer::~ImageViewer()
{
    delete m_label;
    delete m_area_label;
    delete m_area;
}

void ImageViewer::setPicture(const QString &path)
{
    m_path = path;
    m_display();
}

void ImageViewer::zoomIn()
{
    m_mode = Fixed;
    m_zoom_factor += 15;

    m_display();
}

void ImageViewer::zoomOut()
{
    m_zoom_factor -= 15;

    if(m_zoom_factor <= 100)
    {
        m_zoom_factor = 100;
        adjust();
        return;
    }

    m_mode = Fixed;
    m_display();
}

void ImageViewer::adjust()
{
    m_zoom_factor = 100;

    m_mode = Adjust;
    m_display();
}

void ImageViewer::m_display()
{
    m_layout->removeWidget(m_label);
    m_layout->removeWidget(m_area);

    // if the mode is Adjust, use a QLabel is preferable. If not, we are gonna use a Scroll Area
    if(m_mode == Adjust)
    {
        m_layout->addWidget(m_label);
        m_area->hide();
        QPixmap pix;
        if(!pix.load(m_path))
        {
            m_label->setPixmap(QPixmap());
            m_label->setText(tr("Impossible to load the picture."));
            return;
        }
        m_label->setPixmap(pix);
    }

    else if(m_mode == Fixed)
    {
        m_area->show();
        m_layout->addWidget(m_area);
        m_area->setAlignment(Qt::AlignCenter);
        QPixmap pix;
        if(!pix.load(m_path))
        {
            m_mode = Adjust;
            m_display();
        }
        // computes the new size
        int n_width = (pix.width() * m_zoom_factor) / 100;
        int n_height = (pix.height() * m_zoom_factor) / 100;
        pix = pix.scaled(n_width, n_height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_area_label->setPixmap(pix);
        m_area_label->resize(pix.size());
        m_area_label->setAlignment(Qt::AlignCenter);

        m_area->setWidget(m_area_label);
    }

    setMinimumSize(100,100);
}

void ImageViewer::resizeEvent(QResizeEvent *ev)
{
    if(m_mode == Adjust)
    {
        // in Adjust mode, the picture has to adapt itselfs at the size of the window
        // if the windows is wider and higher than the pixmap, displays it to its normal size
        QPixmap pix;
        pix.load(m_path);
        if(width() > pix.width() && height() > pix.height())
            m_display();    // re initialize the display
        // if at least one of the window side is smaller, resize the picture
        if(width() < pix.width() || height() < pix.height())
        {
            pix = pix.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            m_label->setPixmap(pix);
        }
    }
}
