#ifndef MAGELLANDEVICEMANAGER_H
#define MAGELLANDEVICEMANAGER_H

#include <QtCore/QHash>

#include <QtGui/QWidget>
#include <QtGui/QListWidget>

#include <Kommodity/GIO/VolumeMonitor>
#include <Kommodity/GIO/Volume>
#include <Kommodity/GIO/Mount>
#include <Kommodity/GIO/Drive>
using namespace Kommodity::GIO;

class DeviceWidget;
class DeviceWidgetList;
class DeviceManager : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceManager(QWidget *parent = 0);

signals:
    void urlChanged(QUrl);

private slots:
    void m_widget_clicked();
    void m_widget_ejected();

    // slots called when the volum monitor sees something
    void m_drive_changed(const VolumeMonitor*, const Drive*) { m_update_list(); }
    void m_drive_connected(const VolumeMonitor*, const Drive*) { m_update_list(); }
    void m_drive_disconnected(const VolumeMonitor*, const Drive*) { m_update_list(); }
    void m_mount_added(const VolumeMonitor*, const Mount*) { m_update_list(); }
    void m_mount_changed(const VolumeMonitor*, const Mount*) { m_update_list(); }
    void m_pre_unmout(const VolumeMonitor*, const Mount*) { m_update_list(); }
    void m_mount_removed(const VolumeMonitor*, const Mount*) { m_update_list(); }
    void m_volume_added(const VolumeMonitor*, const Volume*) { m_update_list(); }
    void m_volume_changed(const VolumeMonitor*, const Volume*) { m_update_list(); }
    void m_volume_removed(const VolumeMonitor*, const Volume*) { m_update_list(); }

private:
    DeviceWidgetList *m_display;
    VolumeMonitor *m_vol_mtr;
    QHash<DeviceWidget*,Mount*> m_mounts;
    QHash<DeviceWidget*,Volume*> m_volumes;

    void m_update_list();
    bool m_lock;

protected:
    virtual void resizeEvent(QResizeEvent *);
};

class QLabel;
class QPushButton;
class QHBoxLayout;
class DeviceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceWidget(QIcon ic, QString text, bool ejectable, QWidget *parent = 0);

signals:
    void eject();
    void clicked();

public slots:
    void setSelected(bool);

private:
    QLabel *m_icon, *m_text;
    QPushButton *m_eject;
    QHBoxLayout *m_layout;

    bool m_selected;

protected:
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *);
};

class QVBoxLayout;
class DeviceWidgetList : public QWidget
{
    Q_OBJECT
public:
    DeviceWidgetList(QWidget *parent = 0);

    void addWidget(DeviceWidget *);
    void clear();

signals:
    void itemClicked(DeviceWidget*);

private slots:
    void m_item_clicked();

private:
    QVBoxLayout *m_layout;
    QList<DeviceWidget*> m_items;
};

#endif // MAGELLANDEVICEMANAGER_H
