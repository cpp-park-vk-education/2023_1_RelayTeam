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

void addressList() {
	const QHostAddress& localhost = QHostAddress(QHostAddress::LocalHost);
	qDebug() << "++++";
	for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
		qDebug() << address.toString();
	}
	qDebug() << "----";
}
