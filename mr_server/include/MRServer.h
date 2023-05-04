#pragma once

#include <QString>
#include <QtNetwork/QAbstractSocket>

class MRServer : public QObject {
private:
	Q_OBJECT
public:
	MRServer();
};
