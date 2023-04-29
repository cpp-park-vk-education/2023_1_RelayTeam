#pragma once

#include <QAbstractSocket>
#include <QString>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>

QString getLocalIP();

QString getMacAddress();

QString getIPv6();
