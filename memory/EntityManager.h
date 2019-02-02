#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <QMap>
#include <QList>
#include "Deduction.h"
#include "Employee.h"
#include "ContinuingRole.h"
#include "Raise.h"
#include "database/StorageManager.h"
#include "integration/AttributePair.h"

/**
 * @brief The EntityManager class
 * Contains the logic for synchronizing data in memory with data in persistent storage.
 * Attributes:
 *  - employeeList : The list of all employees in cuNICS
 *  - storageManager : The object which communicates directly with persistent storage
 *
 *
 *
 * @author Daniel Homer
 */
class EntityManager
{
private:
    QMap<quint32, Employee *>* employees;
    QList<Raise*>* raises;
    //StorageManager storageManager;

public:
    EntityManager();
    ~EntityManager();
    StorageManager storageManager;
    bool addDeduction(quint32 rtype, Deduction *d);
    bool addEmployee(Employee *e);
    bool addEmployeeRole(quint32 eid, EmployeeRole *er);
    bool addLeavePeriod(ContinuingRole *cr, LeavePeriod *lp);
    bool applyRaise(Raise *r);
    Employee* getEmployee(quint32 eid);
    QList<Employee*> getEmployeeList();
    QList<Employee*> getEmployeeListFiltered(quint32 attNum);
    bool updateEmployee(Employee* e);
    bool updateEmployeeAttributes(quint32 eid, QList<AttributePair*>* attributes);
    bool updateEmployeeRole(EmployeeRole* er);
};

#endif // ENTITYMANAGER_H
