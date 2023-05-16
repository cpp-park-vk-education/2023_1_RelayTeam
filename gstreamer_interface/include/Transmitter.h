#pragma once

#include <QObject>

#include "Session.h"

class Transmitter : public Session {
private:
public:
	Transmitter() : Session(QHostAddress(), -1, -1) {}

public slots:
	void onStartSession() override {}

	void onKillSession() override {}
};
