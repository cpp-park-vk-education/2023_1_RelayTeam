#pragma once
#include "DeviceWidget.h"
#include <QtSql>
#include <QVBoxLayout>

class DBManager {
private:
	static bool instance_exist;

	void createDB();

	QSqlDatabase sql_data_base;

public:
	const QString db_name = "MultiRelayData.sqlite";

	const QString db_type = "QSQLITE";

	DBManager();

	~DBManager();

	void addDevice(QString& name, int volume);

	void getDevices(QVBoxLayout* device_layout);

	void saveDeviceChanges(DeviceWidget* device);

	void addTestEntries();
};
