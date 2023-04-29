#include "networkTools.h"

QString getLocalIP() {
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

QString getMacAddress() {
	foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces()) {
		// Return only the first non-loopback MAC Address
		if (!(netInterface.flags() & QNetworkInterface::IsLoopBack)) {
			return netInterface.hardwareAddress();
		}
	}
	return QString();
}

QString getIPv6() {
	QString local_ipv6;
	QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
	foreach (QNetworkInterface interface, interfaces) {
		QList<QNetworkAddressEntry> entries = interface.addressEntries();
		foreach (QNetworkAddressEntry entry, entries) {
			if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol) {
				local_ipv6 = entry.ip().toString();
			}
		}
	}
	return local_ipv6;
}
