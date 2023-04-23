#include "Publisher.h"

Publisher::Publisher(const QString& device_name, QWidget* parent)
	: QObject(parent), server(), hostname(&server), provider(&server, &hostname), service() {
	service.setType("_mrelay-connect._tcp.local.");
	service.setName(device_name.toUtf8());
	service.setPort(5353);
	qDebug() << QString(QSysInfo::kernelType());
	QMap<QByteArray, QByteArray> attributes;
	attributes["OS_type"] = QSysInfo::kernelType().toUtf8().toLower();
	//	attributes["mID"] = QSysInfo::machineUniqueId();
	service.setAttributes(std::move(attributes));
	provider.update(service);
	connect(&hostname, &QMdnsEngine::Hostname::hostnameChanged, this, &Publisher::onHostnameChanged);
	connect(&server, &QMdnsEngine::Server::messageReceived, this, &Publisher::onMessageReceived);
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
	qDebug() << "local ip address: " << local_ip;
	return local_ip;
}

void Publisher::onHostnameChanged(const QByteArray& hostname) {
	qDebug() << QString("Hostname changed to ") + (QString(hostname));
}

void Publisher::onMessageReceived(const QMdnsEngine::Message& message_received) {
	if (message_received.transactionId() != 1264) {
		return;
	}
	if (message_received.queries().size() != 1) {
		return;
	}
	if (message_received.queries().first().name() == QString("request_local_ip.").toUtf8()) {
		qDebug() << "got direct request for local ip from: " << message_received.address();
		QMdnsEngine::Message message_send;
		QMdnsEngine::Query query;
		query.setName(getLocalIP().toUtf8());
		query.setType(8234);
		message_send.addQuery(query);
		message_send.reply(message_received);
		message_send.setTransactionId(3663);
		server.sendMessage(message_send);
	}
}
