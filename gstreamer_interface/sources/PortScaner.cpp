#include <PortScaner.h>

bool PortScaner::portIsBusy(QString ip, qint32 port) {
    QTcpSocket* socket = new QTcpSocket(this);
    socket->connectToHost(ip, port);
    qint8 msecs_delay_time = 100;
    if (socket->waitForConnected(msecs_delay_time)) {
        socket->disconnectFromHost();
        return true;
    } else {
        return false;
    }
}

qint32 PortScaner::getPort() {
    qint32 startRangeSearch = 4000;
    qint32 stopRangeSearch = 5000;
    for (qint32 i = startRangeSearch; i < stopRangeSearch; i++) {
        if (!this->portIsBusy("localhost", i)) {
            return i;
        };
    }
}
