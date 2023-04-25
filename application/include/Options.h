#pragma once

#include <QString>

struct Options {
	qint32 ID;
	QString device_name;

	Options() : ID(-1), device_name("Undefined") {}
	Options(qint32 ID_, QString device_name_) : ID(ID_), device_name(device_name_) {}

	void update(qint32 ID_, const QString& device_name_) {
		ID = ID_;
		device_name = device_name_;
	}

	void update(qint32 ID_, const QString&& device_name_) {
		ID = ID_;
		device_name = std::move(device_name_);
	}
};
