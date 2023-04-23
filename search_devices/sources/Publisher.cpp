#include "Publisher.h"

Publisher::Publisher(QWidget* parent) : QObject(parent), server(), hostname(&server), provider(&server, &hostname), service() {
	service.setType("_mrelay-connect._tcp.local.");
	service.setName(QSysInfo::machineHostName().toUtf8());
	service.setPort(5353);
	qDebug() << QString(QSysInfo::kernelType());
	QMap<QByteArray, QByteArray> attributes;
	attributes["OS_type"] = QSysInfo::kernelType().toUtf8().toLower();
	//	attributes["mID"] = QSysInfo::machineUniqueId();
	service.setAttributes(std::move(attributes));
	provider.update(service);
	connect(&hostname, &QMdnsEngine::Hostname::hostnameChanged, this, &Publisher::onHostnameChanged);
}

void Publisher::onHostnameChanged(const QByteArray& hostname) {
	qDebug() << QString("Hostname changed to ") + (QString(hostname));
}
