#include "EntityControl.h"
#include "ContinuingRole.h"
#include "TermRole.h"

#include <iostream>
#include <iterator>

EntityControl::EntityControl()
{
    employees = new QMap<quint32, Employee*>();
    roles = new QMap<quint32, EmployeeRole*>();
    leaves = new QMap<quint32, LeavePeriod*>();
    raises = new QMap<quint8, Raise*>();
    deductions = new QMap<quint32, Deduction*>();
    role_deduction_map = new QMap<quint8, QList<quint32>*>();
    dbControl = new DatabaseControl();

    /* Builds the lists for acceptable employee role types */
    role_deduction_map->insert(EmployeeRole::TYPE_STAFF | EmployeeRole::TIME_FULL | EmployeeRole::DURATION_TERM, new QList<quint32>());
    role_deduction_map->insert(EmployeeRole::TYPE_STAFF | EmployeeRole::TIME_FULL | EmployeeRole::DURATION_CONT, new QList<quint32>());
    role_deduction_map->insert(EmployeeRole::TYPE_STAFF | EmployeeRole::TIME_PART | EmployeeRole::DURATION_TERM, new QList<quint32>());
    role_deduction_map->insert(EmployeeRole::TYPE_STAFF | EmployeeRole::TIME_PART | EmployeeRole::DURATION_CONT, new QList<quint32>());
    role_deduction_map->insert(EmployeeRole::TYPE_FACULTY | EmployeeRole::TIME_FULL | EmployeeRole::DURATION_TERM, new QList<quint32>());
    role_deduction_map->insert(EmployeeRole::TYPE_FACULTY | EmployeeRole::TIME_FULL | EmployeeRole::DURATION_CONT, new QList<quint32>());
    role_deduction_map->insert(EmployeeRole::TYPE_TA | EmployeeRole::TIME_PART | EmployeeRole::DURATION_TERM, new QList<quint32>());
    role_deduction_map->insert(EmployeeRole::TYPE_RA | EmployeeRole::TIME_PART | EmployeeRole::DURATION_TERM, new QList<quint32>());

    dbControl->fillLists(employees, roles, leaves, raises, deductions, role_deduction_map);
}

EntityControl::~EntityControl()
{
//todo delete objects
    qDeleteAll(*employees);
    qDeleteAll(*raises);
    qDeleteAll(*deductions);
    qDeleteAll(*role_deduction_map);
    deductions->clear();
    role_deduction_map->clear();
    employees->clear();
    roles->clear();
    leaves->clear();
    raises->clear();


    delete employees;
    delete roles;
    delete leaves;
    delete raises;
    delete deductions;
    delete role_deduction_map;
    delete dbControl;
}

void EntityControl::printObjects()
{
    for (int i=0; i<employees->values().size(); ++i) {
        Employee* emp = employees->values().at(i);

        std::cout << emp->getEmployeeID() << ", "
                  << emp->getGivenName().toStdString() << ", "
                  << emp->getFamilyName().toStdString() << ", "
                  << emp->getAddress().getHouseNumber().toStdString() << ", "
                  << emp->getAddress().getStreet().toStdString() << ", "
                  << emp->getAddress().getPostalCode().toStdString() << ", "
                  << emp->getAddress().getCity().toStdString() << ", "
                  << emp->getAddress().getProvince().toStdString() << ", "
                  << emp->getAddress().getCountry().toStdString() << ", "
                  << emp->getUserID().toStdString() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "roles:" << std::endl;
    for (int i=0; i<roles->size(); ++i) {
        EmployeeRole* role = roles->values().at(i);

        std::cout << role->getRoleID() << ", "
                  << role->getRoleName().toStdString() << ", "
                  << role->getStartDate().toString().toStdString() << ", "
                  << (role->isTermRole() ? ((TermRole*)role)->getEndDate().toString().toStdString() : "N/A") << ", "
                  << role->getSalary() << ", "
                  << (int)role->getType() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "leaves:" << std::endl;
    for (int i=0; i<leaves->size(); ++i) {
        LeavePeriod* leave = leaves->values().at(i);

        std::cout << leave->getLeaveID() << ", "
                  << leave->getStartDate().toString().toStdString() << ", "
                  << leave->getEndDate().toString().toStdString() << ", "
                  << leave->getPercent() << std::endl;
    }
    std::cout << std::endl;

    for (int i=0; i<raises->size(); ++i) {

    }
}

Employee* EntityControl::createEmployee(QString userName, QString givenName, QString familyName, Address* address)
{
   quint32 new_id = dbControl->getNextEmployeeNumber();

   Employee* new_employee = new Employee(userName, new_id, givenName, familyName, address);

   employees->insert(new_id, new_employee);

   //p[ut role and employee into database
   dbControl->addEmployee(*new_employee);

   return(new_employee);
}

EmployeeRole* EntityControl::createFacultyRole(Employee& emp, QString title, double baseSalary, Date* start)
{
    ContinuingRole* new_role = new ContinuingRole(dbControl->getNextRoleNumber(), title, baseSalary, start, EmployeeRole::TIME_FULL, EmployeeRole::TYPE_FACULTY);

    emp.addEmployeeRole(new_role);
    roles->insert(new_role->getRoleID(), new_role);
    dbControl->addRole(emp, *new_role);

    return (new_role);
}

EmployeeRole* EntityControl::createFacultyRole(Employee& emp, QString title, double baseSalary, Date* start, Date* end)
{
    TermRole* new_role = new TermRole(dbControl->getNextRoleNumber(), title, baseSalary, start, end, EmployeeRole::TIME_FULL, EmployeeRole::TYPE_FACULTY);

    emp.addEmployeeRole(new_role);
    roles->insert(new_role->getRoleID(), new_role);
    dbControl->addRole(emp, *new_role);

    return (new_role);
}

EmployeeRole* EntityControl::createRARole(Employee& emp, QString title, double baseSalary, Date* start, Date* end)
{
    TermRole* new_role = new TermRole(dbControl->getNextRoleNumber(), title, baseSalary, start, end, EmployeeRole::TIME_PART, EmployeeRole::TYPE_RA);

    emp.addEmployeeRole(new_role);
    roles->insert(new_role->getRoleID(), new_role);
    dbControl->addRole(emp, *new_role);

    return (new_role);
}

EmployeeRole* EntityControl::createStaffRole(Employee& emp, QString title, double baseSalary, Date* start)
{
    ContinuingRole* new_role = new ContinuingRole(dbControl->getNextRoleNumber(), title, baseSalary, start, EmployeeRole::TIME_PART, EmployeeRole::TYPE_STAFF);

    emp.addEmployeeRole(new_role);
    roles->insert(new_role->getRoleID(), new_role);
    dbControl->addRole(emp, *new_role);

    return (new_role);
}

EmployeeRole* EntityControl::createStaffRole(Employee& emp, QString title, double baseSalary, Date* start, Date* end)
{
    TermRole* new_role = new TermRole(dbControl->getNextRoleNumber(), title, baseSalary, start, end, EmployeeRole::TIME_FULL, EmployeeRole::TYPE_STAFF);

    emp.addEmployeeRole(new_role);
    roles->insert(new_role->getRoleID(), new_role);
    dbControl->addRole(emp, *new_role);

    return (new_role);
}

EmployeeRole* EntityControl::createStaffRole(Employee& emp, QString title, double baseSalary, Date* start, bool isPartTime)
{
    if (isPartTime) {
        ContinuingRole* new_role = new ContinuingRole(dbControl->getNextRoleNumber(), title, baseSalary, start, EmployeeRole::TIME_PART, EmployeeRole::TYPE_STAFF);

        emp.addEmployeeRole(new_role);
        roles->insert(new_role->getRoleID(), new_role);
        dbControl->addRole(emp, *new_role);

        return (new_role);
    } else {
        return (createStaffRole(emp, title, baseSalary, start));
    }
}

EmployeeRole* EntityControl::createStaffRole(Employee& emp, QString title, double baseSalary, Date* start, Date* end, bool isPartTime)
{
    if (isPartTime) {
        TermRole* new_role = new TermRole(dbControl->getNextRoleNumber(), title, baseSalary, start, end, EmployeeRole::TIME_PART, EmployeeRole::TYPE_STAFF);

        emp.addEmployeeRole(new_role);
        roles->insert(new_role->getRoleID(), new_role);
        dbControl->addRole(emp, *new_role);

        return (new_role);
    } else {
        return (createStaffRole(emp, title, baseSalary, start, end));
    }
}

EmployeeRole* EntityControl::createTARole(Employee& emp, QString title, double baseSalary, Date* start, Date* end)
{
    TermRole* new_role = new TermRole(dbControl->getNextRoleNumber(), title, baseSalary, start, end, EmployeeRole::TIME_PART, EmployeeRole::TYPE_TA);

    emp.addEmployeeRole(new_role);
    roles->insert(new_role->getRoleID(), new_role);
    dbControl->addRole(emp, *new_role);

    return (new_role);
}
