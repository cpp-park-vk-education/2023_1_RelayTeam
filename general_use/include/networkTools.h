#pragma once

#include <QAbstractSocket>
#include <QString>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>

QHostAddress getLocalIPv4();

QHostAddress getLocalIPv6();

QString getMacAddress();

QHostAddress getIPv6();

QString processIPv6(const QHostAddress& ipv6_address);
