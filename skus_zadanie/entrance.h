        #pragma once

        #include <QMainWindow>
        #include <QMessageBox>

        #include "ui_entrance.h"
        #include "User.h"
        #include "Company.h"
        #include <mysql_connection.h>
        #include <mysql_driver.h>
        #include <cppconn/driver.h>
        #include <cppconn/exception.h>
        #include <cppconn/prepared_statement.h>
        #include <cppconn/statement.h>
        using std::map;

        class entrance : public QMainWindow
        {
            Q_OBJECT

        public:
            entrance(QWidget* parent = nullptr);
            ~entrance();
            bool getActiveUsr(int i) const { return activeUsr[i]; }
            int getIndexActiveUsr() { return indexActiveUsr; }
            QVector<Guest> getGuest() { return guest; }
            QVector<Company> getCompany() { return company; }
            QVector<Employee> getEmployee() { return employee; }
            void setActiveUsrDefault() 
            { 
                std::fill(std::begin(activeUsr), std::end(activeUsr), false);
            }
            void initializeUsers(sql::Connection* con, sql::Statement* stmt, sql::ResultSet* res);

        private:
            Ui::entranceClass ui;
            QVector<Guest> guest;
            QVector<Company> company;
            QVector<Employee> employee;
            map<QString, QString> guestLog;
            map<QString, QString> companyLog;
            map<QString, QString> employeeLog;
 
            int indexActiveUsr;
            bool activeUsr[3] = {false, false, false};  // 0 true - guest, 1 true - company, 2 true - employee

        private slots:
            void on_loginPushButton_clicked();

        signals:
            void loginCompleted();
        };