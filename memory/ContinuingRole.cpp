#include "ContinuingRole.h"

#include <QDebug>
/**
 * @brief ContinuingRole::ContinuingRole
 *      Should only be used by the DatabaseControl to make a
 * ContinuingRole based on what is stored in the database.
 *
 * @param roleID : the id of the role for reference
 * @param roleName : the name of the role
 * @param roleSalary : the base salary of the role
 * @param startDate : the role start date
 * @param type : the type value of the role, differs from the other
 *            constructor, this is the value that represents CONT/TERM,
 *            STAFF/FACULTY/TA/RA and FULL/PART time.
 */
ContinuingRole::ContinuingRole(quint32 roleID, QString roleName, double roleSalary, Date* startDate, quint8 type)
    : EmployeeRole::EmployeeRole(roleID, roleName, roleSalary, startDate, type)
{

}

/**
 * @brief ContinuingRole::ContinuingRole
 *      Should only be used by the EntityControl. If a role is
 * required to be made make use of the member functions in EntityControl
 * as they handle everything that is required when making a new role.
 *
 * @param roleID : the id of the role for reference
 * @param roleName : the name of the role
 * @param roleSalary : the base salary of the role
 * @param startDate : the role start date
 * @param time : EmployeeRole::TIME_FULL or EmployeeRole::TIME_PART
 * @param type : EmployeeRole::TYPE_FACULTY or EmployeeRole::TYPE_STAFF
 */
ContinuingRole::ContinuingRole(quint32 roleID, QString roleName, double roleSalary, Date* startDate, quint8 time, quint8 type)
    : EmployeeRole::EmployeeRole(roleID, roleName, roleSalary, startDate, DURATION_CONT, time, type)
{

}

/**
 * @brief ContinuingRole::~ContinuingRole
 * Cleans up the leaves of the ContinuingRole if there are any.
 */
ContinuingRole::~ContinuingRole()
{
    qDeleteAll(leaves);
}

/**
 * @brief ContinuingRole::addLeavePeriod
 *      Adds a leave to the role.
 * @param newLeave : the leave to add
 */
void ContinuingRole::addLeavePeriod(LeavePeriod* newLeave)
{
    leaves.append(newLeave);
}

/**
 * @brief ContinuingRole::getLeavePeriods
 * @return the role's leave periods
 */
QList<LeavePeriod*>& ContinuingRole::getLeavePeriods()
{
    return(leaves);
}
