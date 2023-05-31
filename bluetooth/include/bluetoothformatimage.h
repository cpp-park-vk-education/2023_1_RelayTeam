#pragma once

#include <QPixmap>
#include <QApplication>
#include <QBuffer>
#include <QDebug>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothsocket.h>
#include <QBluetoothAddress>
#include <QBluetoothDeviceInfo>
#include <QBluetoothServer>
#include <QBluetoothLocalDevice>

class BluetoothFormatImage : public QObject
{
    Q_OBJECT
private:
    QByteArray data_image;
public:
    BluetoothFormatImage(const QByteArray &data);
    BluetoothFormatImage(QPixmap &map_image);
    QByteArray getDataImage();
};
