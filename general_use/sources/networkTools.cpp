#include "networkTools.h"

QHostAddress getLocalIPv4() {
	QHostAddress local_ip;
	const QHostAddress& localhost = QHostAddress(QHostAddress::LocalHost);
	for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
		if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
			local_ip = address;
		}
	}
	qDebug() << "Your local ipv4 is: " << local_ip.toString();
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
