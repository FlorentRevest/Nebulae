#include "magellan-devicemanager.h"

#include <QtGui/QLabel>
#include <QtGui/QPainter>
#include <QtGui/QPushButton>
#include <QtGui/QBoxLayout>

#include "Kommodity/GIO/MountOperation"
#include "Kommodity/GIO/Cancellable"
#include "Kommodity/GIO/ThemedIcon"
#include "Kommodity/GIO/File"

#include <QtDebug>

DeviceManager::DeviceManager(QWidget *parent) :
    QWidget(parent)
{

    // initialize display
    m_display = new DeviceWidgetList(this);

    // initialise Kommodity volume monitor
    m_vol_mtr = new VolumeMonitor();
    connect(m_vol_mtr, SIGNAL(driveChanged(const VolumeMonitor * , const Drive * )), this, SLOT(m_drive_changed(const VolumeMonitor * , const Drive * )));
    connect(m_vol_mtr, SIGNAL(driveConnected(const VolumeMonitor * , const Drive * )), this, SLOT(m_drive_connected(const VolumeMonitor * , const Drive * )));
    connect(m_vol_mtr, SIGNAL(driveDisconnected(const VolumeMonitor * , const Drive * )), this, SLOT(m_drive_disconnected(const VolumeMonitor * , const Drive * )));
    connect(m_vol_mtr, SIGNAL(mountAdded(const VolumeMonitor * , const Mount * )), this, SLOT(m_mount_added(const VolumeMonitor * , const Mount * )));
    connect(m_vol_mtr, SIGNAL(mountChanged(const VolumeMonitor * , const Mount * )), this, SLOT(m_mount_changed(const VolumeMonitor * , const Mount * )));
    connect(m_vol_mtr, SIGNAL(preUnmount(const VolumeMonitor * , const Mount * )), this, SLOT(m_pre_unmount(const VolumeMonitor * , const Mount * )));
    connect(m_vol_mtr, SIGNAL(mountRemoved(const VolumeMonitor * , const Mount * )), this, SLOT(m_mount_removed(const VolumeMonitor * , const Mount * )));
    connect(m_vol_mtr, SIGNAL(volumeAdded(const VolumeMonitor * , const Volume * )), this, SLOT(m_volume_added(const VolumeMonitor * , const Volume * )));
    connect(m_vol_mtr, SIGNAL(volumeChanged(const VolumeMonitor * , const Volume * )), this, SLOT(m_volume_changed(const VolumeMonitor * , const Volume * )));
    connect(m_vol_mtr, SIGNAL(volumeRemoved(const VolumeMonitor * , const Volume * )), this, SLOT(m_volume_removed(const VolumeMonitor * , const Volume * )));    

    m_lock = false;
    m_update_list();
}

void DeviceManager::m_widget_clicked()
{
    if(m_lock)
        return;
    m_lock = true;

    DeviceWidget *w = qobject_cast<DeviceWidget*>(sender());
    // ok, is it a mount ?
    Mount *mnt = m_mounts.value(w);
    if(mnt != 0)
        emit urlChanged(mnt->getRoot().getUri());
    else
    {
        // ok... it's a volume !
        delete mnt;
        Volume *vol = m_volumes.value(w);
        if(vol != 0)
        {
            MountOperation mount;
            vol->mount(Mount::MountNone, mount);
        }
        else // WTF is it ?!
            delete vol;
    }

    m_lock = false;
}

void DeviceManager::m_widget_ejected()
{
    DeviceWidget *w = qobject_cast<DeviceWidget*>(sender());
    Mount *mnt = m_mounts.value(w);
    if(mnt != 0)
        mnt->eject(Mount::UnmountForce);
}

void DeviceManager::m_update_list()
{
    if(m_lock)
        return;
    m_lock = true;

    emit driveListChanged();

    m_display->clear();

    m_mounts.clear();
    m_volumes.clear();

    foreach(Mount *mnt, m_vol_mtr->getMounts())
    {
        QIcon ic = QIcon();
        ThemedIcon *t = dynamic_cast<ThemedIcon*>(mnt->getIcon());
        if(t) {
            ic = QIcon::fromTheme(t->getNames().first());
        }
        DeviceWidget *w = new DeviceWidget(ic, mnt->getName(), true, this);
        connect(w, SIGNAL(clicked()), this, SLOT(m_widget_clicked()));
        connect(w, SIGNAL(eject()), this, SLOT(m_widget_ejected()));
        m_display->addWidget(w);
        m_mounts.insert(w, mnt);
    }

    foreach(Volume *vol, m_vol_mtr->getVolumes())
    {
        if(vol->getMount() == 0)  {
            QIcon ic = QIcon();
            ThemedIcon *t = dynamic_cast<ThemedIcon*>(vol->getIcon());
            if(t) {
                ic = QIcon::fromTheme(t->getNames().first());
            }
            DeviceWidget *w = new DeviceWidget(ic, vol->getName(), false, this);
            connect(w, SIGNAL(eject()), this, SLOT(m_widget_ejected()));
            connect(w, SIGNAL(clicked()), this, SLOT(m_widget_clicked()));
            m_display->addWidget(w);
            m_volumes.insert(w, vol);
        }
    }

    m_lock = false;
}

void DeviceManager::resizeEvent(QResizeEvent *)
{
    m_display->resize(size());
}

DeviceWidget::DeviceWidget(QIcon ic, QString text, bool ejectable, QWidget *parent) : QWidget(parent)
{
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0,0,0,0);
    setLayout(m_layout);

    m_icon = new QLabel(this);
    m_icon->setPixmap(ic.pixmap(24, 24));
    m_layout->addWidget(m_icon);

    m_text = new QLabel(this);
    m_text->setText(text);
    m_layout->addWidget(m_text, 1, Qt::AlignLeft);

    if(ejectable)
    {
        m_eject = new QPushButton(QIcon::fromTheme("media-eject"), QString(), this);
        m_eject->setToolTip(tr("Eject this media"));
        m_eject->setFlat(true);
        connect(m_eject, SIGNAL(clicked()), this, SIGNAL(eject()));
        m_layout->addWidget(m_eject);
    }
    m_selected = false;

    setMaximumHeight(24);
}

void DeviceWidget::setSelected(bool selected)
{
    m_selected = selected;
    repaint(rect());
}

void DeviceWidget::mouseReleaseEvent(QMouseEvent *)
{
    emit clicked();
}

void DeviceWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(m_selected)
    {
        painter.setBrush(QBrush(Qt::blue));
        painter.drawRect(rect());
    }
}

DeviceWidgetList::DeviceWidgetList(QWidget *parent) : QWidget(parent)
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setAlignment(m_layout, Qt::AlignTop);
    setLayout(m_layout);
}

void DeviceWidgetList::addWidget(DeviceWidget *w)
{
    if(!m_items.isEmpty())
    {
        m_layout->removeWidget(m_items.last());
        m_layout->addWidget(m_items.last());
    }

    m_items.append(w);
    m_layout->addWidget(w, 1, Qt::AlignTop);
    connect(w, SIGNAL(clicked()), this, SLOT(m_item_clicked()));
}

void DeviceWidgetList::clear()
{
    foreach(DeviceWidget *item, m_items)
    {
        m_layout->removeWidget(item);
        m_items.removeOne(item);
        delete item;
    }
}

void DeviceWidgetList::m_item_clicked()
{
    DeviceWidget *it = qobject_cast<DeviceWidget*>(sender());

    if(!it)
        return;

    foreach(DeviceWidget *item, m_items)
    {
        if(it == item)
            item->setSelected(true);
        else
            item->setSelected(false);
    }
    emit itemClicked(it);
}
