#include "bluetoothformatimage.h"
#include <QDir>

BluetoothFormatImage::BluetoothFormatImage(const QByteArray &data)
{
    data_image = data;
}

BluetoothFormatImage::BluetoothFormatImage(QPixmap &map_image) {
    QBuffer buffer(&data_image);
    buffer.open(QIODevice::WriteOnly);
    map_image.save(&buffer, "PNG");
}

QByteArray BluetoothFormatImage::getDataImage() {
    return data_image;
}
