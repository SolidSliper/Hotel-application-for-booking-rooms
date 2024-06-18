#pragma once

#include <QtWidgets/QMainWindow>
#include <QStandardItem>
#include <QFileDialog>
#include "ui_hotel.h"
#include "entrance.h"
#include "editDatabase.h"
#include "Room.h"

class hotel : public QMainWindow
{
    Q_OBJECT

public:
    hotel(QWidget* parent = nullptr);
    ~hotel();

private:
    Ui::hotelClass ui;
    entrance* e;
    editDatabase* databaseEdit;

    int countRooms, countOneBed, countTwoBed, countFourBed, actUsrIndx, selectedRoomIndex = -1;
    bool typeUsr[3] = { false, false, false };
    bool admin = false;
    bool recept = false;

    QString language = "English";
    QVector<Employee> emp;
    QVector<QString> types_of_rooms = { "oneBedded", "twoBedded", "fourBedded" };
    QVector<Room> rooms;
    QVector<QToolButton*> buttonRoom;
    QDate selectedDateRes;

    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    sql::ResultSet* res;

    void fillRooms();
    void saveRooms();
    void listReservations();
    void userReservations();
    void checkReservations();
    void createListItem(int index, Room& room);
    void confirmGuestReservation(const QDate& selectedDate, const QColor& textColor);
    void confirmRegularReservation(const QColor& textColor);
    void cancelAdminReservation(Room& room, bool true_or_false, int i);
    void cancelGuestReservation(Room& room, int i);
    void setBlackReservations(const Room& room);

    bool isEmployee()
    {
        if (typeUsr[2] == true)
            return true;
        else
            return false;
    }
    bool isCompany()
    {
        if (typeUsr[1] == true)
            return true;
        else
            return false;
    }
    bool isGuest()
    {
        if (typeUsr[0] == true)
            return true;
        else
            return false;
    }
    double roomPrice()
    {
        if (selectedRoomIndex < 6)
            return 20;
        else if (selectedRoomIndex > 5 && selectedRoomIndex < 12)
            return 15;
        else
            return 10;
    }
    QString roomType()
    {
        if (selectedRoomIndex < 6)
            return types_of_rooms[0];
        else if (selectedRoomIndex > 5 && selectedRoomIndex < 12)
            return types_of_rooms[1];
        else
            return types_of_rooms[2];
    }

private slots:
    void showWindow();
    void dateReservationSelected(QListWidgetItem* item);
    void handleButtonRoomCheck();

    void on_confirmPushButton_clicked();
    void on_cancelPushButton_clicked();
    void on_pushButtonInfo_clicked();
    void on_pushButtonUa_clicked();
    void on_pushButtonEng_clicked();
    void on_actionLogOut_triggered();
    void on_actionEditUsers_triggered();
    void on_actionInvoice_triggered();
};