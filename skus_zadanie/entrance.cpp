#include "entrance.h"

entrance::entrance(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

entrance::~entrance()
{}

void entrance::on_loginPushButton_clicked()
{
    if (ui.passwordLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Login", "Enter password!");
        return;
    }
    QString login = ui.loginComboBox->currentText();
    // Пошук у guestLog
    auto guestIt = guestLog.find(login);
    if (guestIt != guestLog.end()) {
        // Ключ знайдено, перевірка значення
        if (guestIt->second == ui.passwordLineEdit->text()) {
            QMessageBox::information(this, "Login", "Successful login!");
            for (int i = 0; i < guest.size(); i++)  // шукаємо індекс активного користувача
            {
                if (guest[i].getLogin() == login)
                    indexActiveUsr = guest[i].getId();
            }
            activeUsr[0] = true;
            emit loginCompleted();
            return;
        }
        else {
            QMessageBox::warning(this, "Login", "Incorrect password!");
            return;
        }
    }

    // Пошук у companyLog
    auto companyIt = companyLog.find(login);
    if (companyIt != companyLog.end()) {
        if (companyIt->second == ui.passwordLineEdit->text()) {
            QMessageBox::information(this, "Login", "Successful login!");
            for (int i = 0; i < company.size(); i++)  // шукаємо індекс активного користувача
            {
                if (company[i].getLogin() == login)
                    indexActiveUsr = company[i].getId();
            }
            activeUsr[1] = true;          
            emit loginCompleted();
            return;
        }
        else {
            QMessageBox::warning(this, "Login", "Incorrect password!");
            return;
        }
    }

    // Пошук у employeeLog
    auto employeeIt = employeeLog.find(login);
    if (employeeIt != employeeLog.end()) {
        // Ключ знайдено, перевірка значення
        if (employeeIt->second == ui.passwordLineEdit->text()) {
            QMessageBox::information(this, "Login", "Successful login!");
            for (int i = 0; i < employee.size(); i++)  // шукаємо індекс активного користувача
            {
                if (employee[i].getLogin() == login)
                    indexActiveUsr = i;
            }
            activeUsr[2] = true;
            emit loginCompleted();
            return;
        }
        else {
            QMessageBox::warning(this, "Login", "Incorrect password!");
            return;
        }
    }
}

void entrance::initializeUsers(sql::Connection* con, sql::Statement* stmt, sql::ResultSet* res)
{
    try 
    {
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM guests");
        while (res->next()) {
            QString login = QString::fromStdString(res->getString("login"));
            QString passwrd = QString::fromStdString(res->getString("passwrd"));
            QString name = QString::fromStdString(res->getString("name"));
            QString surname = QString::fromStdString(res->getString("surname"));
            QString billing_address = QString::fromStdString(res->getString("billing_address"));
            int guest_id = res->getInt("id");
            qDebug() << guest_id;

            guestLog[login] = passwrd;
            Guest g = Guest(name, surname, login, billing_address, guest_id);
            guest.insert(guest_id, g);
        }

        res = stmt->executeQuery("SELECT * FROM companies");
        while (res->next())     // Запис юзернеймів та паролів компаній
        {
            QString login = QString::fromStdString(res->getString("login"));
            QString passwrd = QString::fromStdString(res->getString("passwrd"));
            QString name = QString::fromStdString(res->getString("name"));
            bool VAT_payer = res->getBoolean("VAT_payer");
            int company_id = res->getInt("id");

            companyLog[login] = passwrd;
            Company c = Company(name, login, VAT_payer, company_id);
            company.insert(company.end(), c);
        }

        res = stmt->executeQuery("SELECT * FROM employee");
        while (res->next())      // Запис юзернеймів та паролів працівників
        {
            QString login = QString::fromStdString(res->getString("login"));
            QString passwrd = QString::fromStdString(res->getString("passwrd"));
            QString name = QString::fromStdString(res->getString("name"));
            QString surname = QString::fromStdString(res->getString("surname"));
            bool administrator = res->getBoolean("administrator");
            bool receptionist = res->getBoolean("receptionist");

            employeeLog[login] = passwrd;
            Employee e = Employee(name, surname, login, administrator, receptionist);
            employee.insert(employee.end(), e);
        }
    }
    catch (sql::SQLException& e) {
        qDebug() << "SQL error: " << e.what();
    }

    for (Guest& g : guest)
        ui.loginComboBox->addItem(g.getLogin());

    for (Company& c : company)
        ui.loginComboBox->addItem(c.getLogin());

    for (Employee& e : employee)
        ui.loginComboBox->addItem(e.getLogin());
}
