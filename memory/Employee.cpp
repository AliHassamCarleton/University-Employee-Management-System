#include "Employee.h"

/**
 * @brief Employee::Employee
 *  Creates an employee with the given attributes.
 *
 * @param userID : the login name of the employee
 * @param empID : the employee id of the employee
 * @param givenName : the first name of the employee
 * @param familyName : the last name of the employee
 * @param address : the address of the employee (do not delete once passed to this constructor)
 */
Employee::Employee(QString userID, quint32 empID, QString givenName, QString familyName, Address* address)
    : User::User(userID)
{
    this->employeeID = empID;
    this->givenName = givenName;
    this->familyName = familyName;
    this->address = address;
}

/**
 * @brief Employee::~Employee
 * Cleans up the roles and the address of the employee
 */
Employee::~Employee()
{
    qDeleteAll(roles);

    delete address;
}

/**
 * @brief Employee::getEmployeeID
 * @return  the employee id of the employee
 */
quint32 Employee::getEmployeeID()
{
    return (employeeID);
}

/**
 * @brief Employee::getGivenName
 * @return the first name of the employee
 */
QString Employee::getGivenName()
{
    return (givenName);
}

/**
 * @brief Employee::getFamilyName
 * @return the last name of the employee
 */
QString Employee::getFamilyName()
{
    return (familyName);
}

/**
 * @brief Employee::getAddress
 * @return a reference to teh employee address
 */
Address& Employee::getAddress()
{
    return (*address);
}

/**
 * @brief Employee::isFaculty
 * @return if the employee is a faculty member
 */
bool Employee::isFaculty(){return isFaculy;}

void Employee::setEmployeeID(quint32 eid)
{
    this->employeeID = eid;
}

void Employee::setGivenName(QString givenName)
{
    this->givenName = givenName;
}

void Employee::setFamilyName(QString familyName)
{
    this->familyName = familyName;
}

/**
 * @brief Employee::addEmployeeRole
 * @param role the role to add to the employee roles
 */
void Employee::addEmployeeRole(EmployeeRole* role)
{
    roles.append(role);
}

/**
 * @brief Employee::getEmployeeRoles
 * @return the list of employee roles
 */
QList<EmployeeRole*>& Employee::getEmployeeRoles()
{
    return (roles);
}

/**
 * @brief Employee::operator <
 * @param other the other employee to compare to
 * @return true if the employee id of this employee is less than the other
 */
bool Employee::operator <(Employee& other)
{
    return (this->employeeID < other.employeeID);
}
