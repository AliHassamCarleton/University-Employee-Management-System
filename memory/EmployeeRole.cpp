#include "EmployeeRole.h"
#include "ContinuingRole.h"
#include "TermRole.h"

#include <QStringBuilder>

const quint8 EmployeeRole::DURATION_MASK = 0b00000001;
const quint8 EmployeeRole::DURATION_CONT = 0b00000001;
const quint8 EmployeeRole::DURATION_TERM = 0b00000000;

const quint8 EmployeeRole::TIME_MASK = 0b00000010;
const quint8 EmployeeRole::TIME_PART = 0b00000000;
const quint8 EmployeeRole::TIME_FULL = 0b00000010;

const quint8 EmployeeRole::TYPE_MASK    = 0b11111100;
const quint8 EmployeeRole::TYPE_STAFF   = 0b00000100;
const quint8 EmployeeRole::TYPE_FACULTY = 0b00001000;
const quint8 EmployeeRole::TYPE_TA      = 0b00001100;
const quint8 EmployeeRole::TYPE_RA      = 0b00010000;

/**
 * @brief EmployeeRole::EmployeeRole
 *      Creates an employee role via database values.
 * @param roleID : the db id of the role
 * @param roleName : the name of the role
 * @param roleSalary : the base salary fo the role
 * @param startDate : the start date of the role
 * @param type : the value stored in the database for type
 */
EmployeeRole::EmployeeRole(quint32 roleID, QString roleName, double roleSalary, Date* startDate, quint8 type)
{
    this->roleName = roleName;
    this->roleSalary = roleSalary;
    this->startDate = startDate;
    this->roleType = type;
    this->roleID = roleID;
}

/**
 * @brief EmployeeRole::EmployeeRole
 *      Creates an employee for the EntityControl
 * @param roleID : the db id of the role
 * @param roleName : the name of the role
 * @param roleSalary : the base salary fo the role
 * @param startDate : the start date of the role
 * @param duration : term or continuing
 * @param time : full or part time
 * @param type : ra/ta/staff/faculty
 */
EmployeeRole::EmployeeRole(quint32 roleID, QString roleName, double roleSalary, Date* startDate, quint8 duration, quint8 time, quint8 type)
{
    this->roleName = roleName;
    this->roleSalary = roleSalary;
    this->startDate = startDate;
    this->roleType = duration | time | type;
    this->roleID = roleID;
}

/**
 * @brief EmployeeRole::~EmployeeRole
 * Cleans up the start date
 */
EmployeeRole::~EmployeeRole()
{
    delete startDate;
}

/**
 * @brief EmployeeRole::isContinuingRole
 * @return if the role is a continuing role
 */
bool EmployeeRole::isContinuingRole()
{
    return ((roleType & DURATION_MASK) == DURATION_CONT);
}

/**
 * @brief EmployeeRole::isTermRole
 * @return if the role is a term role
 */
bool EmployeeRole::isTermRole()
{
    return ((roleType & DURATION_MASK) == DURATION_TERM);
}

/**
 * @brief EmployeeRole::isFullTime
 * @return if the role is full time
 */
bool EmployeeRole::isFullTime()
{
    return ((roleType & TIME_MASK) == TIME_FULL);
}

/**
 * @brief EmployeeRole::isPartTime
 * @return if the role is part time
 */
bool EmployeeRole::isPartTime()
{
    return ((roleType & TIME_MASK) == TIME_PART);
}

/**
 * @brief EmployeeRole::getStartDate
 * @return get a reference to the start date
 */
Date& EmployeeRole::getStartDate()
{
    return (*startDate);
}

/**
 * @brief EmployeeRole::getSalary
 * @return the base salary of the salary
 */
double EmployeeRole::getSalary()
{
    return (roleSalary);
}

/**
 * @brief EmployeeRole::getType
 * @return get the role type attribute
 */
quint8 EmployeeRole::getType()
{
    return (roleType);
}

/**
 * @brief EmployeeRole::getRoleID
 * @return the role db id
 */
quint32 EmployeeRole::getRoleID()
{
    return (roleID);
}

/**
 * @brief EmployeeRole::getRoleName
 * @return the role name
 */
QString EmployeeRole::getRoleName()
{
    return (roleName);
}

void EmployeeRole::setRoleID(quint32 roleID)
{
    this->roleID = roleID;
}

void EmployeeRole::setSalary(double salary)
{
    this->roleSalary = salary;
}

QString EmployeeRole::toString()
{
    QString time = (isPartTime() ? "Part-time " : "Full-time ");
    QString duration = (isContinuingRole() ? "continuing " : "term ");
    QString type;
    if ((roleType & EmployeeRole::TYPE_MASK) == EmployeeRole::TYPE_FACULTY) {
        type = "Faculty";
    } else if ((roleType & EmployeeRole::TYPE_MASK) == EmployeeRole::TYPE_STAFF) {
        type = "Staff";
    } else if ((roleType & EmployeeRole::TYPE_MASK) == EmployeeRole::TYPE_TA) {
        type = "TA";
    } else {
        type = "RA";
    }

    return (time % duration % type);
}

/**
 * @brief EmployeeRole::rolePosition
 * @return the position value
 */
quint32 EmployeeRole::rolePosition()
{
    return (roleType & TYPE_MASK);
}

/**
 * @brief EmployeeRole::operator <
 * @param other the role to compare to
 * @return true if this role id is less than the other's id
 */
bool EmployeeRole::operator<(EmployeeRole& other)
{
    return (this->roleID < other.roleID);
}
