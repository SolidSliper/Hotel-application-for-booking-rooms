#pragma once
#include <QString>
#include <QMessageBox>

class Room
{
private:
	bool free; // стан кімнати
	bool new_reservation; // резервація
	bool isGuest; // гість чи компанія?
	int guest_id;
	int company_id; // індекс гостя, який має резервацію на цю кімнату
	int room_index;
	double price; // ціна за 1 особу
	QString type; // тип кімнати - oneBedded, twoBedded, fourBedded
	QDate date; // дата, на яку кімната є зарезервована 
public:
	Room() {}
	Room(bool f, bool n, int gi, int ci, int r_i, double p, QString t, QDate d, bool is)
		: free(f), new_reservation(n), guest_id(gi), company_id(ci), room_index(r_i), price(p), type(t), date(d), isGuest(is) {}

	Room(bool f, bool n, int gi, int r_i, double p, QString t, QDate d, bool is)
		: free(f), new_reservation(n), guest_id(gi), room_index(r_i), price(p), type(t), date(d), isGuest(is) {}

	Room(bool f, bool n, short int ci, int r_i, double p, QString t, QDate d, bool is)
		: free(f), new_reservation(n), company_id(ci), room_index(r_i), price(p), type(t), date(d), isGuest(is) {}
	inline void print();
	bool getFree() const { return free; }
	bool getNew() const { return new_reservation; }
	int getCompanyId() { return company_id; }
	int getGuestId() { return guest_id; }
	int getRoomIndex() { return room_index; }
	double getPrice() { return price; }
	QString getType() { return type; }
	QDate getDate() const { return date; }
	bool getIsGuest() { return isGuest; }
	void setNew(bool n) { new_reservation = n; }
	void setCompanyId(int i) { company_id = i; }
	void setGuestId(int i) { guest_id = i; }
	void setDate(QDate d) { date = d; }
	void setFree(bool f) { free = f; }
	void setIsGuest(bool is) { isGuest = is; }

};
inline void Room::print()
{
	qDebug() << free << " " << new_reservation << " " << company_id << " " << guest_id << " " << price << " " << type << " " << date << " " << isGuest << " " << room_index;
}