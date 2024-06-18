#pragma once
#include <QString>
#include <map>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>

class User
{
protected:
    QString name, surname, login;

public:
    User() : name(""), surname("") {}
    User(QString n, QString s, QString u) : name(n), surname(s), login(u) {}
    QString getLogin() { return login; }
    QString getName() { return name; }
    QString getSurname() { return surname; }
};

class Employee : public User
{
private:
    bool administrator;
    bool receptionist;

public:
    Employee() : User(), administrator(false), receptionist(false) {}
    Employee(QString n, QString s, QString u, bool a, bool r) : User(n, s, u), administrator(a), receptionist(r) {}
    bool getAdm() { return administrator; }
    bool getRecep() { return receptionist; }
    inline void print();
};

inline void Employee::print()
{
    qDebug() << name << " " << surname << " " << administrator << " " << receptionist;
}

class Guest : public User
{
private:
    QString billing_address;
    int guest_id;

public:
    Guest() : User(), billing_address("") {}
    Guest(QString n, QString s, QString u, QString b, int g) : User(n, s, u), billing_address(b), guest_id(g) {}
    QString getBill() { return billing_address; }
    int getId() { return guest_id; }
    inline void print();
};

inline void Guest::print()
{
    qDebug() << name << " " << surname << " " << billing_address;
}