#pragma once
#include "User.h"

class Guest : public User
{
private:
    QString billing_address;

public:
    Guest() : User(), billing_address("") {}
    Guest(QString n, QString s, QString u, QString b) : User(n, s, u), billing_address(b) {}
    QString getBill() { return billing_address; }
    inline void print();
};

inline void Guest::print()
{
    qDebug() << name << " " << surname << " " << billing_address;
}