#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "ui_editDatabase.h"
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>

using namespace sql;

class editDatabase : public QMainWindow
{
	Q_OBJECT

public:
	editDatabase(QWidget *parent = nullptr);
	~editDatabase();

	void getDatabase(mysql::MySQL_Driver* driverHotel, Connection* conHotel, Statement* stmtHotel, ResultSet* resHotel)
	{
		driver = driverHotel;
		con = conHotel;
		stmt = stmtHotel;
		res = resHotel;
	}

private:
	Ui::editDatabaseClass ui;
	mysql::MySQL_Driver* driver;
	Connection* con;
	Statement* stmt;
	ResultSet* res;

private slots:
	void on_deletePushButton_clicked();
	void on_addPushButton_clicked();
	void on_actionSave_triggered();
	void on_comboBoxTable_currentIndexChanged();
};
