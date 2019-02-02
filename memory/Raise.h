#ifndef RAISE_H
#define RAISE_H

#include "Date.h"

#define FLAT_AMOUNT_RAISE false
#define PERCENT_RAISE     true

/**
 * @brief The Raise class
 *
 * @author Michael van Dyk
 */
class Raise
{
private:
    quint8 role;
    Date* date;
    bool percent;
    double amount;

public:
    Raise(quint8 role, Date* date, bool percent, double amount);
    ~Raise();

    Date& getApplyDate();
    quint8 getRole();
    bool isPercent();
    double getAmount();

    bool operator<(Raise& other);
};

#endif // RAISE_H
