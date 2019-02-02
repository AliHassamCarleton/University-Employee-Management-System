#ifndef TRANSACTIONFILECREATOR_H
#define TRANSACTIONFILECREATOR_H

#include "AttributePair.h"
#include "memory/Date.h"

#include <fstream>

/**
 * @brief The TransactionFileCreator class
 *
 *  Used to create a transaction file for the integration process.
 *
 * @author Michael van Dyk
 */
class TransactionFileCreator
{
public:
    TransactionFileCreator();
    ~TransactionFileCreator();

    static void makeSampleTransactionFile();

    void addEmployee(quint32 emp_num, QString login_name, QString given_name,
                     QString family_name, QString house_num, QString street_name,
                     QString postal_code, QString city, QString province, QString country);
    void addEmployeeRole(quint32 emp_num, QString roleName, double roleSalary,
                         Date& startDate, quint8 roleType);
    void addEmployeeRole(quint32 emp_num, QString roleName, double roleSalary,
                         Date& startDate, Date& endDate, quint8 roleType);
    void addLeavePeriod(quint32 emp_num, quint8 role_num, Date& start, Date& end, double percent);

    void updateBasic(quint32 emp_num, quint32 attr_count, AttributePair* attributes);
    void updatePay(quint32 emp_num, quint8 role_num, double amount);

    void applyRaise(Date& apply_date, quint8 role_type, bool percent, double amount);

private:
    std::fstream file; // the file connection being written to.

};

#endif // TRANSACTIONFILECREATOR_H
