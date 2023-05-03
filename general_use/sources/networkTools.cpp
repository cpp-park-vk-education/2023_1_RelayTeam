#include "networkTools.h"

QHostAddress getLocalIPv4() {
	QHostAddress local_ip;
	QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
	// Iterate through all the IP addresses and print them
	foreach (const QHostAddress& address, info.addresses()) {
		if (address.protocol() == QAbstractSocket::IPv4Protocol) {
			local_ip = address;
		}
	}
	return local_ip;
}

QHostAddress getLocalIPv6() {
	QHostAddress local_ip;
	QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
	// Iterate through all the IP addresses and print them
	foreach (const QHostAddress& address, info.addresses()) {
		if (address.protocol() == QAbstractSocket::IPv6Protocol) {
			local_ip = address;
		}
	}
	return local_ip;
}

QString getMacAddress() {
	foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces()) {
		// Return only the first non-loopback MAC Address
		if (!(netInterface.flags() & QNetworkInterface::IsLoopBack)) {
			return netInterface.hardwareAddress();
		}
	}
	return QString();
}

QHostAddress getIPv6() {
	QHostAddress local_ipv6;
	QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
	foreach (QNetworkInterface interface, interfaces) {
		QList<QNetworkAddressEntry> entries = interface.addressEntries();
		foreach (QNetworkAddressEntry entry, entries) {
			if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol) {
				local_ipv6 = entry.ip();
			}
		}
	}
	return local_ipv6;
}

void addressList() {
	const QHostAddress& localhost = QHostAddress(QHostAddress::LocalHost);
	qDebug() << "++++";
	for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
		qDebug() << address.toString();
	}
	qDebug() << "----";
}

QString processIPv6(const QHostAddress& ipv6_address) {
	//	QString ipv6_string = ipv6_address.toString();
	//	ipv6_string = ipv6_string.left(24) + QString("%wlan0");
	//	ipv6_string = QString("[") + ipv6_string + QString("]");
	return ipv6_address.toString();
}

bool portIsBusy(QString ip, qint32 port) {
        assert(port >= 1024);
        assert(port <= 49151);
        QTcpSocket* socket = new QTcpSocket();
        socket->connectToHost(ip, port);
        qint8 msecs_delay_time = 10;
        if (socket->waitForConnected(msecs_delay_time)) {
                socket->disconnectFromHost();
                return true;
        } else {
                return false;
        }
}

qint32 getPort(qint32 startRangeSearch, qint32 stopRangeSearch, QString ip) {
        assert(startRangeSearch < stopRangeSearch);
        for (qint32 i = startRangeSearch; i < stopRangeSearch; i++) {
                if (!portIsBusy(ip, i)) {
                        return i;
                };
        }
        return -1;
}
