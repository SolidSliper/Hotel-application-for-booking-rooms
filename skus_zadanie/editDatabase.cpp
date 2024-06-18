#include "editDatabase.h"

editDatabase::editDatabase(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

editDatabase::~editDatabase()
{}

void editDatabase::on_deletePushButton_clicked()
{
    ui.tableWidget->removeRow(ui.spinBoxRow->value() - 1);
    ui.spinBoxRow->setRange(1, ui.tableWidget->rowCount());
}

void editDatabase::on_addPushButton_clicked()
{
    ui.tableWidget->insertRow(ui.tableWidget->rowCount());
    ui.spinBoxRow->setRange(1, ui.tableWidget->rowCount());
}

//void editDatabase::on_actionSave_triggered()
//{
//    try {
//        // �������� �������� ������� rooms
//        QString trun = "TRUNCATE TABLE " + ui.comboBoxTable->currentText();
//
//        stmt = con->createStatement();
//        stmt->execute(trun.toStdString());
//        delete stmt;
//
//        // �������� ������� ��������
//        int columnCount = ui.tableWidget->columnCount();
//
//        // ������� ��������� ����� INSERT
//        QString queryStr = "INSERT INTO " + ui.comboBoxTable->currentText() + " (";
//        for (int col = 0; col < columnCount; ++col) {
//            queryStr += ui.tableWidget->horizontalHeaderItem(col)->text();
//            if (col < columnCount - 1) {
//                queryStr += ", ";
//            }
//        }
//        queryStr += ") VALUES (";
//        for (int col = 0; col < columnCount; ++col) {
//            queryStr += "?";
//            if (col < columnCount - 1) {
//                queryStr += ", ";
//            }
//        }
//        queryStr += ")";
//
//        PreparedStatement* pstmt = con->prepareStatement(queryStr.toStdString());
//
//        // ������ ����� �� ����� �������
//        for (int row = 0; row < ui.tableWidget->rowCount(); ++row) {
//            for (int col = 0; col < columnCount; ++col) {
//                QTableWidgetItem* item = ui.tableWidget->item(row, col);
//                if (item) {
//                    pstmt->setString(col + 1, item->text().toStdString());
//                }
//                else {
//                    pstmt->setString(col + 1, "");
//                }
//            }
//            pstmt->executeUpdate();
//        }
//
//        delete pstmt;
//    }
//    catch (SQLException& e) {
//        QMessageBox::critical(this, "Database Error", QString::fromStdString(e.what()));
//    }
//}

void editDatabase::on_actionSave_triggered()
{
    try {
        // �������� �������� ������� rooms
        QString trun = "TRUNCATE TABLE " + ui.comboBoxTable->currentText();

        stmt = con->createStatement();
        stmt->execute(trun.toStdString());
        delete stmt;

        // �������� ������� ��������
        int columnCount = ui.tableWidget->columnCount();

        // ������� ��������� ����� INSERT
        QString queryStr = "INSERT INTO " + ui.comboBoxTable->currentText() + " (";
        for (int col = 0; col < columnCount; ++col) {
            queryStr += ui.tableWidget->horizontalHeaderItem(col)->text();
            if (col < columnCount - 1) {
                queryStr += ", ";
            }
        }
        queryStr += ") VALUES (";
        for (int col = 0; col < columnCount; ++col) {
            queryStr += "?";
            if (col < columnCount - 1) {
                queryStr += ", ";
            }
        }
        queryStr += ")";

        PreparedStatement* pstmt = con->prepareStatement(queryStr.toStdString());

        // ������ ����� �� ����� �������
        for (int row = 0; row < ui.tableWidget->rowCount(); ++row) {
            for (int col = 0; col < columnCount; ++col) {
                QTableWidgetItem* item = ui.tableWidget->item(row, col);
                if (item && !item->text().isEmpty()) {
                    pstmt->setString(col + 1, item->text().toStdString());
                }
                else {
                    pstmt->setNull(col + 1, sql::DataType::INTEGER); // ������������ �������� NULL ��� ������ �������
                }
            }
            pstmt->executeUpdate();
        }

        delete pstmt;
    }
    catch (SQLException& e) {
        QMessageBox::critical(this, "Database Error", QString::fromStdString(e.what()));
    }
}

void editDatabase::on_comboBoxTable_currentIndexChanged()
{
    try
    {
        QString query = "SELECT * FROM " + ui.comboBoxTable->currentText();
        stmt = con->createStatement();
        res = stmt->executeQuery(query.toStdString());

        // �������� ������� ����� ���������� ����� �����
        ui.tableWidget->clearContents();
        ui.tableWidget->setRowCount(0);

        // �������� ������� �������� � ���������� ���������
        ResultSetMetaData* meta = res->getMetaData();
        int columnCount = meta->getColumnCount();

        ui.tableWidget->setColumnCount(columnCount);
        QStringList headers;
        for (int i = 1; i <= columnCount; ++i)
        {
            headers << QString::fromStdString(meta->getColumnName(i));
        }

        ui.tableWidget->setHorizontalHeaderLabels(headers);

        // ������ ����� �� �������
        int rowCount = 0;
        while (res->next())
        {
            ui.tableWidget->insertRow(rowCount);
            for (int col = 0; col < columnCount; ++col)
            {
                QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(res->getString(col + 1)));
                ui.tableWidget->setItem(rowCount, col, item);
            }
            ++rowCount;
        }

        for (int col = 0; col < columnCount; ++col)
        {
            ui.tableWidget->resizeColumnToContents(col);
        }

        ui.spinBoxRow->setRange(1, rowCount - 1);
    }
    catch (SQLException& e) {
        QMessageBox::critical(this, "Database Error", QString::fromStdString(e.what()));
    }
}
