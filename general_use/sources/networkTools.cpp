#include "networkTools.h"

#include <QNetworkInterface>

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

QString processIPv6(const QHostAddress& ipv6_address) {
    //	QString ipv6_string = ipv6_address.toString();
    //	ipv6_string = ipv6_string.left(24) + QString("%wlan0");
    //	ipv6_string = QString("[") + ipv6_string + QString("]");
    return ipv6_address.toString();
}

bool portIsBusy(const QHostAddress& ipv4_address, qint32 port) {
    Q_ASSERT(port >= 1024);
    Q_ASSERT(port <= 49151);
    QTcpSocket* socket = new QTcpSocket();
    socket->connectToHost(ipv4_address, port);
    qint8 msecs_delay_time = 10;
    if (socket->waitForConnected(msecs_delay_time)) {
        socket->disconnectFromHost();
        return true;
    } else {
        return false;
    }
}

qint32 getPort(qint32 startRangeSearch, qint32 stopRangeSearch, QHostAddress ip_host_address) {
    Q_ASSERT(startRangeSearch < stopRangeSearch);
    for (qint32 i = startRangeSearch; i < stopRangeSearch; i++) {
        if (!portIsBusy(ip_host_address, i)) {
            return i;
        };
        if (i - startRangeSearch >= 10) {
            qDebug() << "Too many time spended to scanning ports - fix it";
        };
    }
    return -1;
}
