#pragma once

#include <QtWidgets/qdialog.h>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include <QtBluetooth/qbluetoothaddress.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothuuid.h>

QT_FORWARD_DECLARE_CLASS(QBluetoothServiceDiscoveryAgent)
QT_FORWARD_DECLARE_CLASS(QListWidgetItem)

class RemoteSelector : public QDialog
{
    Q_OBJECT

public:
    explicit RemoteSelector(const QBluetoothAddress &localAdapter, QWidget *parent = nullptr);
    ~RemoteSelector();

    void startDiscovery(const QBluetoothUuid &uuid);
    void stopDiscovery();
    QBluetoothServiceInfo service() const;

private:

    QBluetoothServiceDiscoveryAgent *m_discovery_agent;
    QBluetoothServiceInfo m_service;
    QMap<QListWidgetItem *, QBluetoothServiceInfo> m_discovered_services;

    QPushButton *cancel_button;
    QVBoxLayout *main_layout;
    QLabel* m_status_label;
    QListWidget* m_remote_devices_list;
    QListWidgetItem* item_device;
private slots:
    void onServiceDiscovered(const QBluetoothServiceInfo &service_info);
    void onDiscoveryFinished();
    void onRemoteDevicesItemActivated(QListWidgetItem *item);
    void onCancelButtonClicked();
};
