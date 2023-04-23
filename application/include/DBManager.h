#pragma once
#include "DeviceWidget.h"
#include "Options.h"
#include <QtSql>
#include <QVBoxLayout>

class DBManager {
private:
	static bool instance_exist;
	QSqlDatabase sql_data_base;

	void createDB();

	void createDefualtOptions();

	void getOptions(Options& options);

	void saveOptionsChanges(const Options& current_options);

	void addTestEntries();

public:
	const QString db_name = "MultiRelayData.sqlite";

	const QString db_type = "QSQLITE";

	DBManager();

	~DBManager();

	void addDevice(QString& name, int volume);

	void getDevices(QVBoxLayout* device_layout);

	void saveDeviceChanges(DeviceWidget* device);
};
