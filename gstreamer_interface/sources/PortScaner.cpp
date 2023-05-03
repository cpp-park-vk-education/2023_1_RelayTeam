#include <PortScaner.h>

bool PortScaner::portIsBusy(QString ip, qint32 port) {
    assert(port >= 1024);
    assert(port <= 49151);
    QTcpSocket* socket = new QTcpSocket(this);
    socket->connectToHost(ip, port);
    qint8 msecs_delay_time = 10;
    if (socket->waitForConnected(msecs_delay_time)) {
        socket->disconnectFromHost();
        return true;
    } else {
        return false;
    }
}

qint32 PortScaner::getPort(qint32 startRangeSearch, qint32 stopRangeSearch, QString ip) {
    assert(startRangeSearch < stopRangeSearch);
    for (qint32 i = startRangeSearch; i < stopRangeSearch; i++) {
        if (!this->portIsBusy(ip, i)) {
            return i;
        };
    }
    return -1;
}
