#include "Publisher.h"

#include <networkTools.h>

#include <QtNetwork/QNetworkInterface>

Publisher::Publisher(const QString& device_name, QWidget* parent)
    : QObject(parent), server(), hostname(&server), provider(&server, &hostname), service() {
    service.setType("_mrelay-connect._tcp.local.");
    service.setName(device_name.toUtf8());
    service.setPort(5353);
    QMap<QByteArray, QByteArray> attributes;
    attributes["OS_type"] = QSysInfo::kernelType().toLower().toUtf8();
    //	attributes["mID"] = QSysInfo::machineUniqueId();
    service.setAttributes(std::move(attributes));
    provider.update(service);
    connect(&hostname, &QMdnsEngine::Hostname::hostnameChanged, this, &Publisher::onHostnameChanged);
    qDebug() << device_name << " published";
}

void Publisher::onHostnameChanged(const QByteArray hostname) {
    qDebug() << QString("Hostname changed to: ") + (QString(hostname));
}

void Publisher::onChangeServiceName(const QString service_name) {
    qDebug() << "changing service name to: " << service_name;
    service.setName(service_name.toUtf8());
    provider.update(service);
}
