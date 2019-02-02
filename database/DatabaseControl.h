#ifndef DATABASECONTROL_H
#define DATABASECONTROL_H

#include <QSqlDatabase>
#include <QIntegerForSize>
#include <string>
#include <QMap>
#include <QList>

#include "memory/Employee.h"
#include "memory/TermRole.h"
#include "memory/ContinuingRole.h"
#include "memory/LeavePeriod.h"
#include "memory/Raise.h"
#include "memory/Deduction.h"

#define DEBUG_DB

/**
 * @brief The DatabaseControl class
 *
 * @author Michael van Dyk
 */
class DatabaseControl
{
private:
    QSqlDatabase db;

    bool buildDatabase();
    bool dropDatabase();
    void fillDatabase();
    quint32 getVariable(QString var_name);
    void setVariable(QString var_name, quint32 val);

public:
    DatabaseControl();
    ~DatabaseControl();

    quint32 getNextEmployeeNumber();
    quint32 getNextRoleNumber();
    quint32 getNextLeaveNumber();
    quint32 getNextDeductionNumber();

    bool addEmployee(Employee& employee);
    bool addRole(Employee& employee, TermRole& role);
    bool addRole(Employee& employee, ContinuingRole& role);
    bool addLeavePeriod(EmployeeRole& role, LeavePeriod& leave);
    bool addDeduction(Deduction& deduction);
    bool addDeductionToRole(Deduction& deduction, quint8 roleDuration, quint8 roleTime, quint8 roleType);

    bool updateEmployee(Employee& employee);
    bool updateEmployeeRole(Employee& employee, EmployeeRole& role);

    void fillLists(QMap<quint32, Employee*>* employees, QMap<quint32, EmployeeRole*>* roles,
                   QMap<quint32, LeavePeriod *>* leaves, QMap<quint8, Raise*>* raises,
                   QMap<quint32, Deduction*>* deductions, QMap<quint8, QList<quint32>*>* role_deduction_map);

#ifdef DEBUG_DB
    void printDB();
#endif
};

#endif // DATABASECONTROL_H
