#pragma once

#include <QAbstractSocket>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>
#include <cassert>

QHostAddress getLocalIPv4();

QHostAddress getLocalIPv6();

QString getMacAddress();

QHostAddress getIPv6();

QString processIPv6(const QHostAddress& ipv6_address);

qint32 getPort(qint32 startRangeSearch = 4000, qint32 stopRangeSearch = 5000, QString ip = "localhost");

bool portIsBusy(QString ip, qint32 port);
