#pragma once

#include <QObject>

#include "Session.h"

class Receiver : public Session {
private:
public:
	Receiver() : Session(QHostAddress(), -1, -1) {}

public slots:
	void onStartSession() override {}

	void onKillSession() override {}
};
