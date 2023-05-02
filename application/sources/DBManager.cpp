#include "DBManager.h"

void DBManager::createDB() {
	qDebug() << "Creating new database.";
	sql_data_base = QSqlDatabase::addDatabase(db_type);
	sql_data_base.setDatabaseName(db_name);
	sql_data_base.open();
	QSqlQuery query;
	query.prepare(
		"create table Device("
		"ID VARCHAR(17) primary key,"
		"name VARCHAR(20),"
		"local_ipv4_address VARCHAR(39),"
		"volume integer)");
	if (!query.exec()) {
		qDebug() << "Error adding Device table to database: " << sql_data_base.lastError().text();
	}
	query.prepare(
		"create table Options("
		"ID integer primary key,"
		"device_name VARCHAR(20),"
		"scale integer"
		")");
	if (!query.exec()) {
		qDebug() << "Error adding Options table to database: " << sql_data_base.lastError().text();
	}

	createDefualtOptions();
	//	addTestEntries();  // TEMP
}

void DBManager::createDefualtOptions() {
	QSqlQuery query;
	query.prepare(
		"INSERT INTO Options("
		"ID,"
		"device_name,"
		"scale)"
		"VALUES(:ID, :device_name, :scale);");
	query.bindValue(":ID", 0);
	query.bindValue(":device_name", QSysInfo::machineHostName());
	query.bindValue(":scale", 100);
	if (!query.exec()) {
		qDebug() << "Error adding default options item to data base." << sql_data_base.lastError().text();
	}
}

void DBManager::getOptions(Options* options) {
	QSqlQuery query;
	if (query.exec("SELECT * FROM Options WHERE ID = 0;")) {
		query.next();
		options->update(query.value("ID").toInt(), query.value("device_name").toString(), query.value("scale").toInt());
	} else {
		qDebug() << "Error making query to get options: " << sql_data_base.lastError().text();
	}
}

void DBManager::saveOptionsChanges(Options* options) {
	QSqlQuery query;
	query.prepare("UPDATE Options SET device_name=:device_name, scale=:scale WHERE ID=:ID");
	query.bindValue(":device_name", options->device_name);
	query.bindValue(":ID", options->ID);
	query.bindValue(":scale", options->scale_factor_new);
	if (!query.exec()) {
		qDebug() << "Error updating Options in data base." << sql_data_base.lastError().text();
	}
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

void DBManager::addDevice(DeviceWidget* device) {  // change to "DeviceWidget* device" later
	QSqlQuery query;
	query.prepare(
		"INSERT INTO Device("
		"ID,"
		"name,"
		"local_ipv4_address,"
		"volume)"
		"VALUES(:ID, :name, :local_ipv4_address, :volume);");
	query.bindValue(":ID", device->ID);
	query.bindValue(":name", device->name);
	query.bindValue(":local_ipv4_address", device->local_ipv4_address.toString());
	query.bindValue(":volume", device->volume);
	if (!query.exec()) {
		qDebug() << "Error adding Device to data base." << sql_data_base.lastError().text();
	}
}

void DBManager::getDevices(QVBoxLayout* device_layout, qreal scale) {
	QSqlQuery query;
	if (query.exec("SELECT * FROM Device")) {
		while (query.next()) {
			DeviceWidget* current_device =
				new DeviceWidget(query.value("ID").toString(), query.value("name").toString(),
								 QHostAddress(query.value("local_ipv4_address").toString()), query.value("volume").toInt(), scale);
			device_layout->addLayout(current_device);
		}
	} else {
		qDebug() << "Error making query to get devices: " << sql_data_base.lastError().text();
	}
}

void DBManager::saveDeviceChanges(DeviceWidget* device) {
	QSqlQuery query;
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
		DeviceWidget* new_device = new DeviceWidget(QString::number(i), QString("device"), QHostAddress(""), 50, 100);
		addDevice(new_device);
	}
}

bool DBManager::instance_exist = false;
