#ifndef EMPLOYEE
#define EMPLOYEE

#include "User.h"
#include "Address.h"
#include "EmployeeRole.h"
#include <QList>

/**
 * @brief The Employee class
 *      Represents the employee data in memory.
 *
 * Attributes:
 *  - employeeID : a unique identifier for an employee
 *  - givenName : the first name of the employee
 *  - familyName : the last name of the employee
 *  - address : the address of the employee
 *  - isFaculty : used to state that the employee is faculty and can only have a single role
 *  - roles : the roles of the employee
 *
 * @author Daniel Homer
 * @author Michael van Dyk
 */
class Employee : public User
{
public:
    Employee(QString userID, quint32 empID, QString givenName, QString familyName, Address* address);
    ~Employee();
    quint32 getEmployeeID();
    QString getGivenName();
    QString getFamilyName();
    Address& getAddress();

    bool isFaculty();

    void setEmployeeID(quint32 eid);
    void setGivenName(QString givenName);
    void setFamilyName(QString familyName);

    void addEmployeeRole(EmployeeRole* role);
    QList<EmployeeRole*>& getEmployeeRoles();

    bool operator <(Employee& other);

private:
    quint32 employeeID;
    QString givenName;
    QString familyName;
    Address* address;

    bool isFaculy;

    QList<EmployeeRole*> roles;
};

#endif // EMPLOYEE

