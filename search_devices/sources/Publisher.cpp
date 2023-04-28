#include "Publisher.h"
#include <QtNetwork/QNetworkInterface>

Publisher::Publisher(const QString& device_name, QWidget* parent)
	: QObject(parent), server(), hostname(&server), provider(&server, &hostname), service() {
	service.setType("_mrelay-connect._tcp.local.");
	service.setName(device_name.toUtf8());
	service.setPort(1234);
	QMap<QByteArray, QByteArray> attributes;
	attributes["OS_type"] = QSysInfo::kernelType().toLower().toUtf8();
	//	attributes["mID"] = QSysInfo::machineUniqueId();
	service.setAttributes(std::move(attributes));
	provider.update(service);
	connect(&hostname, &QMdnsEngine::Hostname::hostnameChanged, this, &Publisher::onHostnameChanged);
	connect(&server, &QMdnsEngine::Server::messageReceived, this, &Publisher::onMessageReceived);
	qDebug() << device_name << " published";
}

QString Publisher::getLocalIP() {
	QString local_ip;
	QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
	// Iterate through all the IP addresses and print them
	foreach (const QHostAddress& address, info.addresses()) {
		if (address.protocol() == QAbstractSocket::IPv4Protocol) {
			local_ip = address.toString();
		}
	}
	return local_ip;
}

QString Publisher::getMacAddress() {
	foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces()) {
		// Return only the first non-loopback MAC Address
		if (!(netInterface.flags() & QNetworkInterface::IsLoopBack)) {
			return netInterface.hardwareAddress();
		}
	}
	return QString();
}

void Publisher::onHostnameChanged(const QByteArray& hostname) {
	qDebug() << QString("Hostname changed to ") + (QString(hostname));
}

void Publisher::onMessageReceived(const QMdnsEngine::Message& message_received) {
	if (message_received.transactionId() != 1264) {
		return;
	}
	QList<QMdnsEngine::Query> queries = message_received.queries();
	if (queries.front().name() == "mrelay-request-local-ip.") {
		qDebug() << "mrelay-request-local-ip. query recieved";
		QMdnsEngine::Message message;
		QMdnsEngine::Query query;
		query.setName("mrelay-answer-local-ip");
		query.setType(2222);
		message.addQuery(query);
		query.setName(queries.back().name());
		query.setType(2222);
		message.addQuery(query);
		query.setName(getLocalIP().toUtf8());
		query.setType(2222);
		message.addQuery(query);
		query.setName(getMacAddress().toUtf8());
		query.setType(2222);
		message.addQuery(query);
		message.reply(message_received);
		message.setTransactionId(3663);
		server.sendMessage(message);
	}
}

void Publisher::onChangeServiceName(const QString& service_name) {
	qDebug() << "changing service name to: " << service_name;
	service.setName(service_name.toUtf8());
	provider.update(service);
}
