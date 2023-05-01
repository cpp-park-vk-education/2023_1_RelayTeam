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
	connect(&server, &QMdnsEngine::Server::messageReceived, this, &Publisher::onMessageReceived);
	qDebug() << device_name << " published";
}

void Publisher::onHostnameChanged(const QByteArray& hostname) {
	qDebug() << QString("Hostname changed to ") + (QString(hostname));
}

void Publisher::onMessageReceived(const QMdnsEngine::Message& message_received) {
	QList<QMdnsEngine::Query> queries = message_received.queries();
	if (message_received.transactionId() == 3645) {
		if (queries.front().name() == "mrelay-start-receiving-session.") {
			QString session_type = queries[1].name().left(queries[1].name().size() - 1);
			qDebug() << "received request for receiving session from" << message_received.address().toString()
					 << "\n session type is: " << session_type;
			emit sendStartReceivingSession(message_received, session_type);
		}
		return;
	}

	if (message_received.transactionId() != 1264) {
		return;
	}
	if (queries.front().name() == "mrelay-request-mac-address.") {
		qDebug() << "mrelay-request-mac-address. query recieved";
		QMdnsEngine::Message message;
		QMdnsEngine::Query query;
		query.setName("mrelay-answer-mac-address");
		query.setType(2222);
		message.addQuery(query);
		query.setName(queries.back().name());
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

void Publisher::onSendPorts(const QMdnsEngine::Message& message_received, qint16 video_port, qint16 audio_port) {
	qDebug() << "sending ports: " << video_port << "  " << audio_port << "to: " << message_received.address();
	QMdnsEngine::Message message;
	QMdnsEngine::Query query;
	query.setName("mrelay-ports-responce");
	query.setType(2222);
	message.addQuery(query);
	query.setName("type value");
	query.setType(video_port);
	message.addQuery(query);
	query.setName("type value");
	query.setType(audio_port);
	message.addQuery(query);
	message.reply(message_received);
	message.setTransactionId(2435);
	server.sendMessage(message);
}
