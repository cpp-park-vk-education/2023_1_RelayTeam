#include "DBManager.h"

void DBManager::addDevice(QString& name, int volume) {
	QSqlQuery query;
	query.prepare(
		"INSERT INTO Device("
		"ID,"
		"name,"
		"volume)"
		"VALUES(:ID, :name, :volume);");
	query.bindValue(":ID", query.lastInsertId());
	query.bindValue(":name", name);
	query.bindValue(":volume", volume);
	if (!query.exec()) {
		qDebug() << "Error adding Device to data base." << sql_data_base.lastError().text();
	}
}

void DBManager::createDB() {
	qDebug() << "Creating new database.";
	sql_data_base = QSqlDatabase::addDatabase(db_type);
	sql_data_base.setDatabaseName(db_name);
	sql_data_base.open();
	QSqlQuery query;
	query.prepare(
		"create table Device("
		"ID integer primary key, "
		"name VARCHAR(20),"
		"volume integer)");
	if (!query.exec()) {
		qDebug() << "Error adding table to database: " << sql_data_base.lastError().text();
	}
	addTestEntries();  // TEMP
}

DBManager::DBManager() {
	Q_ASSERT(!instance_exist && "Single instance DBManager created more then once.");  // single instance, but inaccessible globally)
	instance_exist = true;

	if (!QFile::exists(db_name)) {
		qDebug() << "Data base doesn't exist.";
		createDB();
	} else {
		sql_data_base = QSqlDatabase::addDatabase(db_type);
		sql_data_base.setDatabaseName(db_name);
	}
	if (!sql_data_base.open()) {
		qDebug() << "Error opening database: " << sql_data_base.lastError().text();
	}
}

DBManager::~DBManager() {
	if (sql_data_base.isOpen()) {
		sql_data_base.close();
	}
	instance_exist = false;
}

void DBManager::getDevices(QVBoxLayout* device_layout) {
	QSqlQuery query;
	if (query.exec("SELECT * FROM Device")) {
		while (query.next()) {
			DeviceWidget* current_device =
				new DeviceWidget(query.value("ID").toUInt(), query.value("name").toString(), query.value("volume").toInt());
			device_layout->addLayout(current_device);
		}
	} else {
		qDebug() << "Error making query to get devices: " << sql_data_base.lastError().text();
	}
}

void DBManager::saveDeviceChanges(DeviceWidget* device) {
	QSqlQuery query;
	//"UPDATE student SET faults = 2 WHERE id =1"
	query.prepare("UPDATE Device SET name=:name, volume=:volume WHERE ID=:ID");
	query.bindValue(":name", device->name);
	query.bindValue(":volume", device->volume);
	query.bindValue(":ID", device->ID);
	if (!query.exec()) {
		qDebug() << "Error updating Device in data base." << sql_data_base.lastError().text();
	}
}

void DBManager::addTestEntries() {
	for (size_t i = 0; i < 8; ++i) {
		QString name = QString("Device ") + QString::number(i);
		addDevice(name, 0);
	}
}

bool DBManager::instance_exist = false;
