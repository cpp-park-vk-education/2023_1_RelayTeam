#include "remoteselector.h"

#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothservicediscoveryagent.h>

QT_USE_NAMESPACE

RemoteSelector::RemoteSelector(const QBluetoothAddress &local_adapter, QWidget *parent)
    : QDialog(parent), m_discovery_agent(new QBluetoothServiceDiscoveryAgent(local_adapter)),
    m_status_label(new QLabel(this)), m_remote_devices_list(new QListWidget(this)) {

    item = nullptr;
    main_layout = new QVBoxLayout(this);
    main_layout->addWidget(m_status_label);
    main_layout->addWidget(m_remote_devices_list);

    cancel_button = new QPushButton(tr("Cancel"), this);
    main_layout->addWidget(cancel_button, 0, Qt::AlignRight);

    connect(m_remote_devices_list, SIGNAL(itemActivated(QListWidgetItem*)),
            this, SLOT(onRemoteDevicesItemActivated(QListWidgetItem*)));
    connect(cancel_button, SIGNAL(clicked()), this, SLOT(onCancelButtonClicked()));
    connect(m_discovery_agent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
            this, SLOT(onServiceDiscovered(QBluetoothServiceInfo)));
    connect(m_discovery_agent, SIGNAL(finished()), this, SLOT(onDiscoveryFinished()));
    connect(m_discovery_agent, SIGNAL(canceled()), this, SLOT(onDiscoveryFinished()));
}

RemoteSelector::~RemoteSelector() {
    if(item != nullptr) {
        delete item;
    }
    delete cancel_button;
    delete main_layout;
    delete m_discovery_agent;
}

void RemoteSelector::startDiscovery(const QBluetoothUuid &uuid) {
    m_status_label->setText(tr("Scanning..."));
    if (m_discovery_agent->isActive()){
        m_discovery_agent->stop();
    }

    m_remote_devices_list->clear();

    m_discovery_agent->setUuidFilter(uuid);
    m_discovery_agent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);

}

void RemoteSelector::stopDiscovery() {
    if (m_discovery_agent){
        m_discovery_agent->stop();
    }
}

QBluetoothServiceInfo RemoteSelector::service() const {
    return m_service;
}

void RemoteSelector::onServiceDiscovered(const QBluetoothServiceInfo &service_info) {
    const QBluetoothAddress address = service_info.device().address();
    for (const QBluetoothServiceInfo &info : qAsConst(m_discovered_services)) {
        if (info.device().address() == address){
            return;
        }
    }

    QString remoteName;
    if (service_info.device().name().isEmpty()){
        remoteName = address.toString();
    }
    else{
        remoteName = service_info.device().name();
    }

    item = new QListWidgetItem(QString::fromLatin1("%1").arg(service_info.serviceName()));

    m_discovered_services.insert(item, service_info);
    m_remote_devices_list->addItem(item);
}


void RemoteSelector::onDiscoveryFinished() {

}

void RemoteSelector::onRemoteDevicesItemActivated(QListWidgetItem *item) {
    qDebug() << "got click" << item->text();
    m_service = m_discovered_services.value(item);
    if (m_discovery_agent->isActive()){
        m_discovery_agent->stop();
    }

    accept();
}

void RemoteSelector::onCancelButtonClicked() {
    reject();
}
