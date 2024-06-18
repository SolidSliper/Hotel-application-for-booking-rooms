#pragma once
#include "User.h"

//class Employee : public User
//{
//private:
//    bool administrator;
//    bool receptionist;
//
//public:
//    Employee() : User(), administrator(false), receptionist(false) {}
//    Employee(QString n, QString s, QString u, bool a, bool r) : User(n, s, u), administrator(a), receptionist(r) {}
//    bool getAdm() { return administrator; }
//    bool getRecep() { return receptionist; }
//    inline void print();
//};
//
//inline void Employee::print()
//{
//    qDebug() << name << " " << surname << " " << administrator << " " << receptionist;
//}