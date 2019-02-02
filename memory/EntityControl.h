#ifndef ENTITYCONTROL_H
#define ENTITYCONTROL_H

#include "database/DatabaseControl.h"
#include <QMap>
#include <QList>

/**
 * @brief The EntityControl class
 *
 * @author Michael van Dyk
 */
class EntityControl
{
private:
    QMap<quint32, Employee *>* employees;
    QMap<quint32, EmployeeRole *>* roles;
    QMap<quint32, LeavePeriod *>* leaves;
    QMap<quint8, Raise *>* raises;
    QMap<quint32, Deduction*>* deductions;
    QMap<quint8, QList<quint32>*>* role_deduction_map;

public:
    DatabaseControl* dbControl;
    EntityControl();
    ~EntityControl();

    void printObjects();

    Employee* createEmployee(QString userName, QString givenName, QString familyName, Address* address);

    EmployeeRole* createFacultyRole(Employee& emp, QString title, double baseSalary, Date* start);
    EmployeeRole* createFacultyRole(Employee& emp, QString title, double baseSalary, Date* start, Date* end);
    EmployeeRole* createRARole(Employee& emp, QString title, double baseSalary, Date* start, Date* end);
    EmployeeRole* createStaffRole(Employee& emp, QString title, double baseSalary, Date* start);
    EmployeeRole* createStaffRole(Employee& emp, QString title, double baseSalary, Date* start, Date* end);
    EmployeeRole* createStaffRole(Employee& emp, QString title, double baseSalary, Date* start, bool isPartTime);
    EmployeeRole* createStaffRole(Employee& emp, QString title, double baseSalary, Date* start, Date* end, bool isPartTime);
    EmployeeRole* createTARole(Employee& emp, QString title, double baseSalary, Date* start, Date* end);
};

#endif // ENTITYCONTROL_H
