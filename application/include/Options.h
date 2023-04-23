#pragma once

#include <QString>

struct Options {
	int ID;
	QString device_name;

	Options() : ID(-1), device_name("Undefined") {}
	Options(int ID_, QString device_name_) : ID(ID_), device_name(device_name_) {}

	void update(int ID_, const QString& device_name_) {
		ID = ID_;
		device_name = device_name_;
	}

	void update(int ID_, const QString&& device_name_) {
		ID = ID_;
		device_name = std::move(device_name_);
	}
};
