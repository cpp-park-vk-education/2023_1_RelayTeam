#pragma once
#include <QDebug>
#include <QProcess>
#include <QString>
#include <QtNetwork/QAbstractSocket>

class SSHServer : public QObject {
private:
	Q_OBJECT
public:
	SSHServer();

	void openConnection() {
		QProcess process;
		QStringList args;
		args << "ssh"
			 << "username@server"
			 << "ls -l";
		process.start("ssh", args);
		process.waitForFinished();

		QString output = process.readAllStandardOutput();
		qDebug() << output;
	}

	void sendMessage() {
		QProcess process;
		QStringList args;
		args << "ssh"
			 << "username@server"
			 << "echo 'Hello, world!' >> message.txt";
		process.start("ssh", args);
		process.waitForFinished();
	}
};
