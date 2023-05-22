#pragma once

#include <QString>

struct Options {
	qint32 ID;
	QString device_name;
	qreal scale_factor;
	qreal scale_factor_new;

	Options() : ID(-1), device_name("Undefined"), scale_factor(1) {}

	Options(qint32 ID_, QString device_name_, qreal scale_factor_) : ID(ID_), device_name(device_name_), scale_factor(scale_factor_) {}

	inline void update(qint32 ID_, const QString& device_name_, qreal scale_factor_) {
		ID = ID_;
		device_name = device_name_;
		scale_factor = scale_factor_;
		scale_factor_new = scale_factor;
	}

	inline void update(qint32 ID_, const QString&& device_name_, qreal scale_factor_) {
		ID = ID_;
		device_name = std::move(device_name_);
		scale_factor = scale_factor_;
		scale_factor_new = scale_factor;
	}

	inline qreal getScale() {
		return scale_factor / 100;
	}
};
