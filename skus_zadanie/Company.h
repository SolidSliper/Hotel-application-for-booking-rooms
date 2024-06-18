#pragma once

class Company
{
private:
    QString name, login;
    bool VAT_payer;
    int company_id;

public:
    Company() : name(""), VAT_payer(false) {}
    Company(QString n, QString u, bool v, int ci) : name(n), login(u), VAT_payer(v), company_id(ci) {}
    bool getVAT() { return VAT_payer; }
    int getId() { return company_id; }
    QString getName() { return name; }
    QString getLogin() { return login; }
};