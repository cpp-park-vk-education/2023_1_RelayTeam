#include "networkTools.h"

QHostAddress getLocalIP() {
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

QString processIPv6(const QHostAddress& ipv6_address) {
	QString ipv6_string = ipv6_address.toString();
	//	ipv6_string = QString("[") + ipv6_string + QString("]");
	qDebug() << ipv6_string;
	return ipv6_string;
}
