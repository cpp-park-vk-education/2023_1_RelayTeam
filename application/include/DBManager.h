#pragma once

#include <QVBoxLayout>
#include <QtNetwork/QHostAddress>
#include <QtSql>

#include "DeviceWidget.h"
#include "Options.h"

class DBManager {
private:
	static bool instance_exist;
	QSqlDatabase sql_data_base;

	void createDB();

	void createDefualtOptions();

	void addTestEntries();

public:
	const QString db_name = "MultiRelayData.sqlite";

	const QString db_type = "QSQLITE";

	DBManager();

	~DBManager();

	void saveOptionsChanges(Options* options);

	void getOptions(Options* options);

	void addDevice(DeviceWidget* device);

	void getDevices(QVBoxLayout* device_layout, qreal scale);

	void saveDeviceChanges(DeviceWidget* device);
};
