#include "hotel.h"

hotel::hotel(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    e = new entrance();
    databaseEdit = new editDatabase();
    e->show();
    ui.dockWidget->hide();

    try {

        // Створення екземпляру драйвера
        driver = sql::mysql::get_mysql_driver_instance();

        // Встановлення з'єднання з базою даних
        con = driver->connect("tcp://127.0.0.1:3306", "root", "ROOT1234ROOT");

        // Вибір бази даних
        con->setSchema("hotel");
    }
    catch (sql::SQLException& e) {
        qDebug() << "SQL error: " << e.what();
    }

    e->initializeUsers(con, stmt, res);
    databaseEdit->getDatabase(driver, con, stmt, res);

    buttonRoom = {
            ui.toolButton101,
            ui.toolButton102,
            ui.toolButton103,
            ui.toolButton104,
            ui.toolButton105,
            ui.toolButton106,
            ui.toolButton201,
            ui.toolButton202,
            ui.toolButton203,
            ui.toolButton204,
            ui.toolButton205,
            ui.toolButton206,
            ui.toolButton301,
            ui.toolButton302,
            ui.toolButton303,
            ui.toolButton304
    };

    ui.pushButtonUa->setStyleSheet("QPushButton#pushButtonUa {"
        "  background-image: url(D:/KEP/project/ua.png);"
        "  background-position: center;"
        "}");
    ui.pushButtonEng->setStyleSheet("QPushButton#pushButtonEng {"
        "  background-image: url(D:/KEP/project/eng.png);"
        "  background-position: center;"
        "}");

    for (auto& button : buttonRoom)
        connect(button, &QToolButton::clicked, this, &hotel::handleButtonRoomCheck);

    fillRooms();

    connect(e, &entrance::loginCompleted, this, &hotel::showWindow);
    connect(ui.listWidget, &QListWidget::itemClicked, this, &hotel::dateReservationSelected);
}

hotel::~hotel()
{
    delete res;
    delete stmt;
    delete con;
    delete e;
    delete databaseEdit;
    saveRooms();
}

void hotel::showWindow() // відкриття головного вікна
{
    e->hide();
    show();
    actUsrIndx = e->getIndexActiveUsr();    // присвоєння значення індекса активного користувача

    for (int i = 0; i < 3; ++i)
        typeUsr[i] = e->getActiveUsr(i);     // тип користувача

    if (language == "English")
        on_pushButtonEng_clicked();
    else
        on_pushButtonUa_clicked();

    if (isEmployee()) // зміна користувацького інтерфейсу для адміна чи працівника рецепції
    {
        emp = e->getEmployee();
        admin = emp[actUsrIndx].getAdm();
        recept = emp[actUsrIndx].getRecep();

        ui.actionInvoice->setVisible(false);
        if (admin == true)
            ui.actionEditUsers->setVisible(true);
        else
            ui.actionEditUsers->setVisible(false);

        listReservations();
    }
    else
    {
        ui.actionEditUsers->setVisible(false);
        ui.actionInvoice->setVisible(true);

        userReservations();
    }
}

void hotel::fillRooms() // метод заповнення класу Room
{
    qDebug() << "Filling rooms...";
    rooms.clear();
    try {
        // Виконання SQL запиту
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM rooms");
        // Обробка результатів запиту
        int i = 0;
        while (res->next()) {
            bool free = res->getBoolean("free");
            bool new_reservation = res->getBoolean("new_reservation");
            int guest_id = res->getInt("guests_id");
            int company_id = res->getInt("companies_id");
            int room_index = res->getInt("room_index");
            double price = res->getDouble("price");
            QString type = QString::fromStdString(res->getString("type_room"));
            QDate date = QDate::fromString(QString::fromStdString(res->getString("date")), "yyyy-MM-dd");
            if (date < QDate::currentDate())
            {
                sql::PreparedStatement* pstmt = con->prepareStatement(
                    "DELETE FROM rooms WHERE id = ?"
                );
                pstmt->setInt(1, i + 1);
                pstmt->execute();
                continue;
            }
            bool isGuest = res->getBoolean("is_guest");

            Room r(free, new_reservation, guest_id, company_id, room_index, price, type, date, isGuest);
            rooms.push_back(r);
            i++;
        }
    }
    catch (sql::SQLException& e) 
    {
        qDebug() << "SQL error: " << e.what();
    }
}

void hotel::checkReservations()   // метод, який встановлює колір дати в календарі
{
    for (Room& room : rooms)
    {
        QDate date = room.getDate();
        QTextCharFormat textCharFormat;

        if (room.getFree() && room.getNew() && room.getRoomIndex() == selectedRoomIndex) // якщо нова резервація, то колір дати в календарі буде пурпурний
        {
            textCharFormat.setForeground(Qt::magenta);
            ui.calendarWidget->setDateTextFormat(date, textCharFormat);
        }

        if (!room.getFree() && !room.getNew() && room.getRoomIndex() == selectedRoomIndex) // якщо підтверджена резервація, то колір дати в календарі буде зелений
        {
            textCharFormat.setForeground(Qt::darkGreen);
            ui.calendarWidget->setDateTextFormat(date, textCharFormat);
        }

        if (!room.getFree() && room.getNew() && room.getRoomIndex() == selectedRoomIndex) // якщо відхилена резервація, то колір дати в календарі буде червоний(окрім вихідних)
        {
            textCharFormat.setForeground(Qt::red);
            ui.calendarWidget->setDateTextFormat(date, textCharFormat);
        }
    }
}

void hotel::createListItem(int index, Room& room)
{
    double newPrice;
    QBrush textBrush;
    QString reservation_status, name, surname, isPayer;
    bool VAT_payer;

    newPrice = room.getPrice();

    if (room.getNew() && room.getFree()) 
    {
        reservation_status = "new_reservation";
        textBrush = QBrush(Qt::magenta);
    }

    if (!room.getNew() && !room.getFree())
    {
        reservation_status = "confirmed_reservation";
        textBrush = QBrush(Qt::darkGreen);
    }

    if (room.getNew() && !room.getFree())
    {
        reservation_status = "canceled_reservation";
        textBrush = QBrush(Qt::red);
    }

    int indxx = actUsrIndx;
    if (isEmployee() && room.getIsGuest())
        indxx = room.getGuestId();
    else if (isEmployee() && !room.getIsGuest())
        indxx = room.getCompanyId();

    if ((room.getIsGuest() && isGuest()) || (room.getIsGuest() && isEmployee()))
    {
        int index;
        QVector<Guest> g = e->getGuest();

        for (int i = 0; i < g.size(); i++)
            if (indxx == g[i].getId())
                index = i;
                    
        name = " " + g[index].getName();
        surname = " " + g[index].getSurname();
    }
    else if ((!room.getIsGuest() && isCompany()) || (!room.getIsGuest() && isEmployee()))
    {
        QVector<Company> c = e->getCompany();

        for (int i = 0; i < c.size(); i++)
        {
            if (indxx == c[i].getId())
            {
                name = " " + c[i].getName();
                VAT_payer = c[i].getVAT();
            }
        }

        if (VAT_payer)
        {
            isPayer = " VAT payer";
            newPrice = room.getPrice() * 0.8;
        }
        else
            isPayer = " not VAT payer";
    }

    QString room_number = " " + buttonRoom[room.getRoomIndex()]->text();

    QString listItem = QString::number(index) + room_number +  " " + room.getType() + " "
        + QString::number(newPrice) + " euro " + room.getDate().toString("yyyy-MM-dd") + " "
        + reservation_status + name + surname + isPayer;

    QListWidgetItem* newItem = new QListWidgetItem(listItem);
    newItem->setForeground(textBrush);

    if (name != "")
        ui.listWidget->addItem(newItem);
    
}

void hotel::userReservations() // Метод випису резервацій для даного гостя
{
    ui.listWidget->clear();  // Чищення QListWidget перед додаванням нових/оновлених елементів

    for (int i = 0; i < rooms.size(); ++i)
    {
        
        if (rooms[i].getGuestId() == actUsrIndx && rooms[i].getIsGuest())
            createListItem(i, rooms[i]);
        
        else if (rooms[i].getCompanyId() == actUsrIndx && !rooms[i].getIsGuest())
            createListItem(i, rooms[i]);

    }
}

void hotel::listReservations() // Метод випису резервацій всіх гостей(для працівників готелю)
{
    ui.listWidget->clear();

    for (int i = 0; i < rooms.size(); ++i)
        createListItem(i, rooms[i]);
}

void hotel::on_confirmPushButton_clicked() // метод підтвердження резервації
{
    if (ui.calendarWidget->selectedDate().isNull())
    {
        QMessageBox::warning(this, "Select date", "Select date for reservation!");
        return;
    }

    if (ui.calendarWidget->selectedDate() < QDate::currentDate())
    {
        QMessageBox::warning(this, "Wrong date", "Choose a correct date!");
        return;
    }

        QDate selectedDate = ui.calendarWidget->selectedDate();
        QColor textColor = ui.calendarWidget->dateTextFormat(selectedDate).foreground().color();

        if (isEmployee()) // якщо працівник готелю, то...
            confirmGuestReservation(selectedDate, textColor);
        else
            confirmRegularReservation(textColor);

        saveRooms();
        checkReservations();
}

void hotel::confirmGuestReservation(const QDate& selectedDate, const QColor& textColor) // метод підтвердження резервації
{
    if (rooms[ui.listWidget->currentRow()].getFree() && rooms[ui.listWidget->currentRow()].getNew())
    {
        rooms[ui.listWidget->currentRow()].setNew(false);
        rooms[ui.listWidget->currentRow()].setFree(false);

        sql::PreparedStatement* pstmt = con->prepareStatement(
            "UPDATE rooms SET free = ?, new_reservation = ? "
            "WHERE id = ?"
        );
        pstmt->setBoolean(1, rooms[ui.listWidget->currentRow()].getFree());
        pstmt->setBoolean(2, rooms[ui.listWidget->currentRow()].getNew());
        pstmt->setInt(3, ui.listWidget->currentRow() + 1);  // Використовуємо `i + 1` як `id`

        pstmt->execute();
        delete pstmt;
    }

    listReservations();
}

void hotel::confirmRegularReservation(const QColor& textColor) // метод резервації
{
    if (textColor == Qt::black && selectedRoomIndex != -1)
    {
        try {
            sql::PreparedStatement* pstmt;

            if (isGuest())
                rooms.append(Room(true, true, actUsrIndx, selectedRoomIndex, roomPrice(), roomType(), ui.calendarWidget->selectedDate(), isGuest()));
            else
                rooms.append(Room(true, true, short int(actUsrIndx), selectedRoomIndex, roomPrice(), roomType(), ui.calendarWidget->selectedDate(), isGuest()));

            if (rooms.last().getIsGuest())
            {
               
                pstmt = con->prepareStatement(
                    "INSERT INTO rooms (free, new_reservation, guests_id, price, type_room, date, is_guest, room_index) "
                    "VALUES (?, ?, ?, ?, ?, ?, ?, ?)"
                );
                pstmt->setInt(3, rooms.last().getGuestId());
            }
            else
            {
                pstmt = con->prepareStatement(
                    "INSERT INTO rooms (free, new_reservation, companies_id, price, type_room, date, is_guest, room_index) "
                    "VALUES (?, ?, ?, ?, ?, ?, ?, ?)"
                );
                pstmt->setInt(3, rooms.last().getCompanyId());
            }
           
            pstmt->setBoolean(1, rooms.last().getFree());
            pstmt->setBoolean(2, rooms.last().getNew());
            pstmt->setDouble(4, rooms.last().getPrice());
            pstmt->setString(5, rooms.last().getType().toStdString());
            pstmt->setString(6, rooms.last().getDate().toString("yyyy-MM-dd").toStdString());
            pstmt->setBoolean(7, rooms.last().getIsGuest());
            pstmt->setInt(8, rooms.last().getRoomIndex());

            pstmt->execute();
        }
        catch (sql::SQLException& e) {
            qDebug() << "SQL error: " << e.what();
        }
    }
    userReservations();
}

void hotel::on_cancelPushButton_clicked() //метод відміни резервації
{
    const QDate selectedDate = ui.calendarWidget->selectedDate();

    if (isEmployee())
    {
        if (rooms[ui.listWidget->currentRow()].getFree())
            cancelAdminReservation(rooms[ui.listWidget->currentRow()], false, ui.listWidget->currentRow());

        if (!rooms[ui.listWidget->currentRow()].getFree() && rooms[ui.listWidget->currentRow()].getNew())
            cancelAdminReservation(rooms[ui.listWidget->currentRow()], true, ui.listWidget->currentRow());
    }
    else
    {
        int i = 0;
        for (Room& room : rooms)
        {
            if (selectedDate == room.getDate() && actUsrIndx == room.getGuestId() && room.getFree() && room.getIsGuest())
            {
                cancelGuestReservation(room, i);
                break;
            }

            if (selectedDate == room.getDate() && actUsrIndx == room.getCompanyId() && room.getFree() && !room.getIsGuest())
            {
                cancelGuestReservation(room, i);
                break;
            }

            i++;
        }
    }
    checkReservations();
}

void hotel::cancelAdminReservation(Room& room, bool true_or_false, int i) // відміна резервації працівником готелю
{
    room.setNew(!true_or_false);
    room.setFree(true_or_false);

    sql::PreparedStatement* pstmt = con->prepareStatement(
        "UPDATE rooms SET free = ?, new_reservation = ? "
        "WHERE id = ?"
    );
    pstmt->setBoolean(1, room.getFree());
    pstmt->setBoolean(2, room.getNew());
    pstmt->setInt(3, i + 1); 

    pstmt->execute();
    delete pstmt;

    if (true_or_false)
    {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "DELETE FROM rooms WHERE id = ?"
        );
        pstmt->setInt(1, i + 1); 
        pstmt->execute();
        delete pstmt; 

        setBlackReservations(room);
        rooms.remove(i);
    }

    listReservations();
    saveRooms();
}

void hotel::cancelGuestReservation(Room& room, int i) // відміна резервації гостем
{
    setBlackReservations(room);
    sql::PreparedStatement* pstmt = con->prepareStatement(
        "DELETE FROM rooms WHERE id = ?"
    );
    pstmt->setInt(1, i);
    pstmt->execute();
    delete pstmt;  

    rooms.remove(i);
    userReservations();
    saveRooms();
}

void hotel::setBlackReservations(const Room& room) // зміна на чорний текст в календарі
{
    const QDate date = room.getDate();
    QTextCharFormat textCharFormat;
    textCharFormat.setForeground(Qt::black);
    ui.calendarWidget->setDateTextFormat(date, textCharFormat);
}

void hotel::dateReservationSelected(QListWidgetItem* item)
{
    QString line = item->text();
    QStringList parts = line.split(" ");
    selectedDateRes = QDate::fromString(parts[5], "yyyy-MM-dd");
    ui.calendarWidget->setSelectedDate(selectedDateRes);
}

void hotel::on_actionEditUsers_triggered()
{
    databaseEdit->show();
}

void hotel::on_actionInvoice_triggered()
{
    QFileDialog dialog(this);
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text Files (*.txt);;All Files (*)"));

    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        int i = 0;
        double countPrice = 0;
        if (isGuest())
        {
            Guest guest = e->getGuest()[actUsrIndx];
            out << "Name: " << guest.getName() << " Surname: " << guest.getSurname() << " Billing adress: " << guest.getBill() << '\n';
            for (Room& room : rooms)
            {
                if (room.getGuestId() == actUsrIndx && room.getIsGuest() == isGuest() && room.getNew() && room.getFree())
                {
                    out << i << " " << room.getType() << " " << room.getDate().toString("yyyy-MM-dd") << " " << room.getPrice() << " euro\n";
                    countPrice += room.getPrice();
                }
                i++;
            }
            out << "To pay: " << countPrice;
        }
        else
        {
            Company company = e->getCompany()[actUsrIndx];
            out << "Name: " << company.getName() << " Vat payer: " << company.getVAT() << '\n';
            for (Room& room : rooms)
            {
                if (room.getGuestId() == actUsrIndx && room.getIsGuest() == isGuest() && room.getNew() && room.getFree())
                {
                    double newPrice;
                    if (company.getVAT())
                        newPrice = room.getPrice() * 0.8;
                    else
                        newPrice = room.getPrice();

                    out << i << " " << room.getType() << " " << room.getDate().toString("yyyy-MM-dd") << " " << newPrice << " euro\n";
                    countPrice += newPrice;
                }
                i++;
            }
            out << "To pay: " << countPrice;
        }
    }
}

void hotel::saveRooms()
{
    try {
        // Виконання SQL запиту для оновлення даних
        stmt = con->createStatement();
        stmt->execute("TRUNCATE TABLE rooms"); // Очистити таблицю перед оновленням даних

        // Підготовка SQL запиту для вставки даних
        sql::PreparedStatement* pstmt;

        // Оновлення даних у таблиці
        for (Room& room : rooms)
        {
            if (room.getIsGuest())
            {
                pstmt = con->prepareStatement(
                    "INSERT INTO rooms (free, new_reservation, guests_id, price, type_room, date, is_guest, room_index) "
                    "VALUES (?, ?, ?, ?, ?, ?, ?, ?)"
                );
                pstmt->setInt(3, room.getGuestId());
            }
            else
            {
                pstmt = con->prepareStatement(
                    "INSERT INTO rooms (free, new_reservation, companies_id, price, type_room, date, is_guest, room_index) "
                    "VALUES (?, ?, ?, ?, ?, ?, ?, ?)"
                );
                pstmt->setInt(3, room.getCompanyId());
            }
            pstmt->setBoolean(1, room.getFree());
            pstmt->setBoolean(2, room.getNew());
            pstmt->setDouble(4, room.getPrice());
            pstmt->setString(5, room.getType().toStdString());
            pstmt->setString(6, room.getDate().toString("yyyy-MM-dd").toStdString());
            pstmt->setBoolean(7, room.getIsGuest());
            pstmt->setInt(8, room.getRoomIndex());

            pstmt->execute();
        }
        delete pstmt;

    }
    catch (sql::SQLException& e) {
        qDebug() << "SQL error: " << e.what();
    }
}

void hotel::handleButtonRoomCheck()
{
    for (Room& room : rooms)
    {
        QTextCharFormat textCharFormat;
        textCharFormat.setForeground(Qt::black);
        ui.calendarWidget->setDateTextFormat(room.getDate(), textCharFormat);
    }
    QToolButton* clickedButton = qobject_cast<QToolButton*>(sender());

    selectedRoomIndex = -1;
    for (int i = 0; i < buttonRoom.size(); i++)
    {
        if (clickedButton == buttonRoom[i])
        {
            selectedRoomIndex = i;
            break;
        }
    }

    for (int i = 0; i < buttonRoom.size(); i++)
        if (i != selectedRoomIndex && buttonRoom[i]->isChecked())
            buttonRoom[i]->setChecked(false);
    

    checkReservations();
}

void hotel::on_pushButtonInfo_clicked()
{
    if (ui.dockWidget->isHidden())
        ui.dockWidget->show();
    else
        ui.dockWidget->hide();
}

void hotel::on_pushButtonUa_clicked()
{
    language = "Ukrainian";

    if (isEmployee())
    {
        ui.usersReservationsGroupBox->setTitle("Резервації гостей");
        ui.labelMain->setText("Перевіряйте резервації гостей у списку, ви також будете бачити записи інших користувачів тільки у календарі, на які забронювати номер не зможете");

    }
    else
    {
        ui.usersReservationsGroupBox->setTitle("Ваші резервації");
        ui.labelMain->setText("Перевіряйте свої резервації у списку, ви також будете бачити записи інших користувачів тільки у календарі, на які забронювати номер не зможете");
    }

    ui.reservationGroupBox->setTitle("Резервація");
    ui.toolBox->setItemText(0, "1 поверх - одномісні номери");
    ui.toolBox->setItemText(1, "2 поверх - двомісні номери");
    ui.toolBox->setItemText(2, "3 поверх - чотиримісні номери");
    ui.pushButtonInfo->setText("Інформація");
    ui.cancelPushButton->setText("Відмінити");
    ui.confirmPushButton->setText("Підтвердити");
    ui.labelMagenta->setText("пурпурна дата - нова резервація");
    ui.labelGreen->setText("зелена дата - підтверджена резервація");
    ui.labelRed->setText("червона дата - відмінена резервація(окрім вихідних)");
    ui.labelContact->setText("Зв'яжіться з нами");
    ui.labelAddress_1->setText("Адреса");
    ui.labelAddress_2->setText("вул. Шевченка 9, Київ, Україна");
    ui.labelSocial->setText("Ми в соцмережах");
    ui.menuFile->setTitle("Файл");
    ui.menuAccount->setTitle("Акаунт");
    ui.actionLogOut->setText("Вихід");
    ui.actionInvoice->setText("Фактура");
    ui.actionEditUsers->setText("Редагувати БД");
}

void hotel::on_pushButtonEng_clicked()
{
    language = "English";

    if (isEmployee())
    {
        ui.usersReservationsGroupBox->setTitle("Guests reservations");
        ui.labelMain->setText("Check guests reservations in the list, you will also see other users' entries only in the calendar, for which you cannot book a room");
    }
    else
    {
        ui.usersReservationsGroupBox->setTitle("Your reservations");
        ui.labelMain->setText("Check your reservations in the list, you will also see other users' entries only in the calendar, for which you cannot book a room");
    }

    ui.reservationGroupBox->setTitle("Reservation");
    ui.toolBox->setItemText(0, "Floor 1 - one bedded rooms");
    ui.toolBox->setItemText(1, "Floor 2 - two bedded rooms");
    ui.toolBox->setItemText(2, "Floor 3 - four bedded rooms");
    ui.pushButtonInfo->setText("Info");
    ui.cancelPushButton->setText("Cancel");
    ui.confirmPushButton->setText("Confirm");
    ui.labelMagenta->setText("*magenta date - reservated");
    ui.labelGreen->setText("*green date - confirmed reservation");
    ui.labelRed->setText("*red date - canceled reservation(except on weekends)");
    ui.labelMain->setText("Check your reservations in the list, you will also see other users' entries only in the calendar, for which you cannot book a room");
    ui.labelContact->setText("Contact us");
    ui.labelAddress_1->setText("Address");
    ui.labelAddress_2->setText("st. Shevchenka 9, Kyiv, Ukraine");
    ui.labelSocial->setText("We are in social media");
    ui.menuFile->setTitle("File");
    ui.menuAccount->setTitle("Account");
    ui.actionLogOut->setText("Log Out");
    ui.actionInvoice->setText("Invoice");
    ui.actionEditUsers->setText("Edit database");
}

void hotel::on_actionLogOut_triggered()
{
    for (int i = 0; i < 3; i++)
        typeUsr[i] = false;

    admin = false;
    recept = false;
    selectedRoomIndex = -1;
    hide();
    e->setActiveUsrDefault();
    e->show();
    saveRooms();
}