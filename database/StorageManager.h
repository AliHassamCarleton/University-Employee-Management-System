#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include "memory/Deduction.h"
#include "memory/Employee.h"
#include "memory/EmployeeRole.h"
#include "memory/ContinuingRole.h"
#include "memory/LeavePeriod.h"
#include "memory/Raise.h"
#include "memory/User.h"

#include "integration/AttributePair.h"

#include "StorageConnection.h"

#include <QMap>

class StorageManager
{
public:
    StorageManager();
    ~StorageManager();

    bool addDeduction(quint32 rtype, Deduction& d);
    bool addEmployee(Employee& e);
    bool addEmployeeRole(quint32 eid, EmployeeRole& er);
    bool addLeavePeriod(ContinuingRole& cr, LeavePeriod& lp);
    bool applyRaise(Raise& raise);

    bool getEmployees(QMap<quint32, Employee *> *employees);

    bool updateEmployee(Employee& e);
    bool updateEmployeeAttributes(quint32 eid, QList<AttributePair*>* attributes);
    bool updateEmployeeRole(EmployeeRole& er);

    quint32 getNextEmployeeNumber();
    quint32 getNextRoleNumber();
    quint32 getNextLeaveNumber();
    quint32 getNextDeductionNumber();

    void printDB();

private:
    StorageConnection connection;

    quint32 getVariable(QString var_name);
    void setVariable(QString var_name, quint32 val);

    bool buildDatabase();
    bool dropDatabase();
    bool fillDatabase();
};

#endif // STORAGEMANAGER_H
