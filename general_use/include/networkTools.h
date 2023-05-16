#pragma once

#include <QAbstractSocket>
#include <QDebug>
#include <QHostAddress>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QtGlobal>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>

QHostAddress getLocalIPv4();

QHostAddress getLocalIPv6();

QString getMacAddress();

QHostAddress getIPv6();

QString processIPv6(const QHostAddress& ipv6_address);

qint32 getPort(qint32 startRangeSearch, qint32 stopRangeSearch, QHostAddress ip_host_address);

bool portIsBusy(const QHostAddress& ipv4_address, qint32 port);
