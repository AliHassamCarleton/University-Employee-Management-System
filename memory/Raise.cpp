#include "Raise.h"

Raise::Raise(quint8 role, Date* date, bool percent, double amount)
{
    this->role = role;
    this->date = date;
    this->percent = percent;
    this->amount = amount;
}

Raise::~Raise()
{
    delete date;
}

bool Raise::operator<(Raise& other)
{
    return (role < other.role);
}

Date& Raise::getApplyDate()
{
    return (*date);
}

quint8 Raise::getRole()
{
    return (role);
}

bool Raise::isPercent()
{
    return (percent);
}

double Raise::getAmount()
{
    return (amount);
}
