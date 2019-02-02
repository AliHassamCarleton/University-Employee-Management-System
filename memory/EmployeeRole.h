#ifndef EMPLOYEEROLE_H
#define EMPLOYEEROLE_H

#include <QString>
#include <QIntegerForSize>

#include "Date.h"

/**
 * @brief The EmployeeRole class
 *      A base class for the employee role structure. Creating a role should be
 * done through the EntityControl since it handles everything that is required
 * to add a role.
 *
 * Attributes:
 *  - roleName : the name of the role
 *  - roleSalary : the base salary of the role
 *  - startDate : the start date of the role
 *  - roleType : a value that store the information relating to duration,
 *              full/part time and if it is a staff/faculty/ra/ta role
 *  - roleID : reference to database entry for quick updates
 *
 * @author Daniel Homer
 * @author Michael van Dyk
 */
class EmployeeRole
{
public:
    ~EmployeeRole();

    bool isContinuingRole();
    bool isTermRole();

    bool isFullTime();
    bool isPartTime();

    Date& getStartDate();
    double getSalary();
    quint8 getType();


    quint32 rolePosition();
    quint32 getRoleID();

    QString getRoleName();

    void setRoleID(quint32 roleID);
    void setSalary(double salary);

    QString toString();

    /**
     * For the retrival of the duration from the roleType attribute.
     */
    static const quint8 DURATION_MASK;
    static const quint8 DURATION_CONT;
    static const quint8 DURATION_TERM;

    /**
     * For the retrival of the time from the roleType attribute.
     */
    static const quint8 TIME_MASK;
    static const quint8 TIME_PART;
    static const quint8 TIME_FULL;

    /**
     * for the retrival of the type from the roleType attribute.
     */
    static const quint8 TYPE_MASK;
    static const quint8 TYPE_STAFF;
    static const quint8 TYPE_FACULTY;
    static const quint8 TYPE_TA;
    static const quint8 TYPE_RA;

protected:
    EmployeeRole(quint32 roleID, QString roleName, double roleSalary, Date* startDate, quint8 type);
    EmployeeRole(quint32 roleID, QString roleName, double roleSalary, Date* startDate, quint8 duration, quint8 time, quint8 type);

private:
    QString roleName;
    double roleSalary;
    Date* startDate;
    quint8 roleType;
    quint32 roleID;

    bool operator<(EmployeeRole& other);
};

#endif // EMPLOYEEROLE_H
