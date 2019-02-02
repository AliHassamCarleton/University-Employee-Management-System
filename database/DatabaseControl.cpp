#include "DatabaseControl.h"

#include <qsqlquery.h>
#include <QtSql>
#include <iostream>
#include <fstream>

#define FORCE_REBUILD

DatabaseControl::DatabaseControl()
{
    static bool open = false;
    if (open) {
        db = QSqlDatabase::database();
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("localhost");
        db.setDatabaseName("./cuNICS.db");
        open = db.open();

#ifdef FORCE_REBUILD
        dropDatabase();
        buildDatabase();
        fillDatabase();
#else
        std::ifstream db("./cuNICS.db");
        if (!db.good()) {
            std::cout << "BUILDING...";
            buildDatabase();
            fillDatabase();
        }
        std::cout << "BUILT" << std::endl;
#endif
    }
}

DatabaseControl::~DatabaseControl()
{
    db.close();
}

quint32 DatabaseControl::getNextEmployeeNumber()
{
    quint32 next = getVariable("emp_num");
    setVariable("emp_num", next + 1);
    return (next);
}

quint32 DatabaseControl::getNextRoleNumber()
{
    quint32 next = getVariable("role_num");
    setVariable("role_num", next + 1);
    return (next);
}

quint32 DatabaseControl::getNextLeaveNumber()
{
    quint32 next = getVariable("leave_num");
    setVariable("leave_num", next + 1);
    return (next);
}

quint32 DatabaseControl::getNextDeductionNumber()
{
    quint32 next = getVariable("deduc_num");
    setVariable("deduc_num", next + 1);
    return (next);
}

bool DatabaseControl::addEmployee(Employee& employee)
{
    QSqlQuery userquery;
    QSqlQuery empquery;

    /*
     * prepare statement for adding an employee, and each of their roles to the db
     */

    userquery.prepare("INSERT INTO user (user_id) VALUES (:user)");
    userquery.bindValue(":user", employee.getUserID());

    if (!userquery.exec()) {
         qDebug() << "ERR: addEmployee(User)" << employee.getUserID();
         return (false);
    }

    empquery.prepare("INSERT INTO employee (employee_id, employee_given_name, employee_family_name, "
                                        "employee_address_number, employee_address_street, "
                                        "employee_address_postal, employee_address_city, "
                                        "employee_address_province, employee_address_country, user_id) "
                                "VALUES (:id, :gname, :fname, "
                                        ":addnum, :addstr, "
                                        ":addpost, :addcity, "
                                        ":addprov, :addcont, :user)");

    empquery.bindValue(":id", employee.getEmployeeID());
    empquery.bindValue(":gname", employee.getGivenName());
    empquery.bindValue(":fname", employee.getFamilyName());
    empquery.bindValue(":addnum", employee.getAddress().getHouseNumber());
    empquery.bindValue(":addstr", employee.getAddress().getStreet());
    empquery.bindValue(":addpost", employee.getAddress().getPostalCode());
    empquery.bindValue(":addcity", employee.getAddress().getCity());
    empquery.bindValue(":addprov", employee.getAddress().getProvince());
    empquery.bindValue(":addcont", employee.getAddress().getCountry());
    empquery.bindValue(":user", employee.getUserID());

    if (!empquery.exec()) {
        qDebug() << "ERR: addEmployee(Employee)" << employee.getGivenName();
        return (false);
    }

    return (true);
}

bool DatabaseControl::addRole(Employee& employee, TermRole& role)
{
    QSqlQuery query;
    query.prepare("INSERT INTO role (role_id, role_start_date, role_end_date, role_salary, role_type, role_name, employee_id) "
                              "VALUES (:rid, :sdate, :edate, :salary, :rtype, :rname, :eid)");

    query.bindValue(":rid", role.getRoleID());
    query.bindValue(":sdate", role.getStartDate().toInteger());
    query.bindValue(":edate", role.getEndDate().toInteger());
    query.bindValue(":salary", role.getSalary());
    query.bindValue(":rtype", role.getType());
    query.bindValue(":rname", role.getRoleName());
    query.bindValue(":eid", employee.getEmployeeID());

    if (!query.exec()) {
        qDebug() << "ERR: addRole(Term)" << query.lastError();
        return (false);
    }

    return(true);
}

bool DatabaseControl::addRole(Employee& employee, ContinuingRole& role)
{
    QSqlQuery query;
    query.prepare("INSERT INTO role (role_id, role_start_date, role_end_date, role_salary, role_type, role_name, employee_id) "
                              "VALUES (:rid, :sdate, 0, :salary, :rtype, :rname, :eid)");

    query.bindValue(":rid", role.getRoleID());
    query.bindValue(":sdate", role.getStartDate().toInteger());
    query.bindValue(":salary", role.getSalary());
    query.bindValue(":rtype", role.getType());
    query.bindValue(":rname", role.getRoleName());
    query.bindValue(":eid", employee.getEmployeeID());

    if (!query.exec()) {
        qDebug() << "ERR: addRole(Continuing)" << query.lastError();
        return (false);
    }

    return(true);
}

bool DatabaseControl::addLeavePeriod(EmployeeRole& role, LeavePeriod& leave)
{
    QSqlQuery query;
    query.prepare("INSERT INTO leave (leave_id, leave_start_date, leave_end_date, leave_percentage, role_id) "
                              "VALUES (:lid, :sdate, :edate, :lper, :rid)");

    query.bindValue(":lid", leave.getLeaveID());
    query.bindValue(":sdate", leave.getStartDate().toInteger());
    query.bindValue(":edate", leave.getEndDate().toInteger());
    query.bindValue(":lper", leave.getPercent());
    query.bindValue(":rid", role.getRoleID());

    if (!query.exec()) {
        qDebug() << "ERR: addLeavePeriod" << query.lastError();
        return (false);
    }

    return (true);
}

bool DatabaseControl::addDeduction(Deduction& deduction)
{
    QSqlQuery query;
    query.prepare("INSERT INTO deduction(deduction_id, deduction_name, deduction_fixed_or_percent, deduction_amount) "
                              "VALUES (:did, :dname, :percent, :amount)");

    query.bindValue(":did", deduction.getDeductionID());
    query.bindValue(":dname", deduction.getDeductionName());
    query.bindValue(":percent", deduction.isPercentage() ? 1 : 0);
    query.bindValue(":amount", deduction.getDeductionAmount());

    if (!query.exec()) {
        qDebug() << "ERR: addDeduction" << query.lastError();
        return (false);
    }
}

bool DatabaseControl::addDeductionToRole(Deduction& deduction, quint8 roleDuration, quint8 roleTime, quint8 roleType)
{
    QSqlQuery query;
    query.prepare("INSERT INTO has_deduction(role_type, deduction_id) "
                              "VALUES (:rtype, :did)");

    query.bindValue(":rtype", roleDuration | roleTime | roleType);
    query.bindValue(":did", deduction.getDeductionID());

    if (!query.exec()) {
        qDebug() << "ERR: addDeductionToRole" << query.lastError();
        return (false);
    }
}

bool DatabaseControl::updateEmployee(Employee& employee)
{
    /*
     * query existing employee data
     * compare attributes for change and builds an update statement
     *
     */
    employee;
    return (false);
}

bool DatabaseControl::updateEmployeeRole(Employee& employee, EmployeeRole& role)
{
    employee;
    role;
    return (false);
}

void DatabaseControl::fillLists(QMap<quint32, Employee*>* employees, QMap<quint32, EmployeeRole*>* roles,
                                QMap<quint32, LeavePeriod *>* leaves, QMap<quint8, Raise*>* raises,
                                QMap<quint32, Deduction*>* deductions, QMap<quint8, QList<quint32>*>* role_deduction_map)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM employee");
    if (!query.exec()) {
        qDebug() << query.lastError();
    }
    while(query.next()) {
        quint32 eid = query.value(0).toUInt();
        QString gname = query.value(1).toString();
        QString fname = query.value(2).toString();
        QString add_num = query.value(3).toString();
        QString add_str = query.value(4).toString();
        QString add_post = query.value(5).toString();
        QString add_city = query.value(6).toString();
        QString add_prov = query.value(7).toString();
        QString add_cont = query.value(8).toString();
        QString uid = query.value(9).toString();
        Address* addr = new Address(add_cont, add_prov, add_city, add_str, add_num, add_post);
        Employee* emp = new Employee(uid, eid, gname, fname, addr);
        employees->insert(eid, emp);
    }

    query.exec("SELECT * FROM raise");
    while(query.next()) {
        Date* raiseDate = new Date(query.value(0).toUInt());
        quint8 role = query.value(1).toUInt();
        bool percent = query.value(2).toUInt() == 1;
        double amount = query.value(3).toDouble();
        raises->insert(role, new Raise(role, raiseDate, percent, amount));
    }

    query.exec("SELECT * FROM role");
    while (query.next()){
        quint32 rid = query.value(0).toUInt();
        Date* sdate = new Date(query.value(1).toUInt());
        Date* edate = new Date(query.value(2).toUInt());
        double salary = query.value(3).toDouble();
        quint8 role = query.value(4).toUInt();
        QString name = query.value(5).toString();
        quint32 eid = query.value(6).toUInt();

        EmployeeRole* new_role = (edate == 0) ? (EmployeeRole*)new ContinuingRole(rid, name, salary, sdate, role)
                                              : (EmployeeRole*)new TermRole(rid, name, salary, sdate, edate, role);

        roles->insert(rid, new_role);
        employees->value(eid)->addEmployeeRole(new_role);
    }

    query.exec("SELECT * FROM leave");
    while (query.next()) {
        quint32 lid = query.value(0).toUInt();
        Date* sdate = new Date(query.value(1).toUInt());
        Date* edate = new Date(query.value(2).toUInt());
        double percent = query.value(3).toDouble();
        quint32 rid = query.value(4).toUInt();

        LeavePeriod* new_leave = new LeavePeriod(lid, sdate, edate, percent);

        leaves->insert(lid, new_leave);
        ((ContinuingRole *)roles->value(rid))->addLeavePeriod(new_leave);
    }

    query.exec("SELECT * FROM deduction");
    while (query.next()) {
        quint32 did = query.value(0).toUInt();
        QString dname = query.value(1).toString();
        bool percent = query.value(2).toUInt() == 1;
        double amount = query.value(3).toDouble();

        Deduction* new_deduction = new Deduction(did, dname, percent, amount);

        //deductions->insert(did, new_deduction);
        delete new_deduction;
    }

    query.exec("SELECT * FROM has_deduction");
    while (query.next()) {
        quint8 rtype = query.value(0).toUInt();
        quint32 deduction_id = query.value(1).toUInt();

        //role_deduction_map->value(rtype)->append(deduction_id);
    }
}


bool DatabaseControl::buildDatabase()
{
    QSqlDatabase::database().transaction();
    QSqlQuery query;

    query.exec("create table user (user_id TEXT primary key NOT NULL)");
    query.exec("create table payroll_specialist "
                  "(user_id TEXT primary key NOT NULL references user(user_id))");
    query.exec("create table employee ("
                  "employee_id INTEGER primary key NOT NULL,"
                  "employee_given_name TEXT NOT NULL,"
                  "employee_family_name TEXT NOT NULL,"
                  "employee_address_number TEXT NOT NULL,"
                  "employee_address_street TEXT NOT NULL,"
                  "employee_address_postal TEXT NOT NULL,"
                  "employee_address_city TEXT NOT NULL,"
                  "employee_address_province TEXT NOT NULL,"
                  "employee_address_country TEXT NOT NULL,"
                  "user_id TEXT NOT NULL references user(user_id))");
    query.exec("create table raise ("
                  "raise_date INTEGER NOT NULL,"
                  "role_type INTEGER NOT NULL,"
                  "raise_fixed_or_percent INTEGER NOT NULL,"
                  "raise_amount REAL NOT NULL,"
                  "primary key (raise_date, role_type))");
    query.exec("create table role ("
                  "role_id INTEGER primary key NOT NULL,"
                  "role_start_date INTEGER NOT NULL,"
                  "role_end_date INTEGER default NULL,"
                  "role_salary REAL NOT NULL,"
                  "role_type INTEGER NOT NULL references raise(role_type),"
                  "role_name TEXT NOT NULL,"
                  "employee_id INTEGER NOT NULL references employee(employee_id))");
    query.exec("create table deduction ("
                  "deduction_id INTEGER primary key NOT NULL,"
                  "deduction_name TEXT NOT NULL,"
                  "deduction_fixed_or_percent INTEGER NOT NULL,"
                  "deduction_amount REAL NOT NULL)");
    query.exec("create table has_deduction ("
                  "role_type INTEGER NOT NULL references role(role_type),"
                  "deduction_id INTEGER NOT NULL references deduction(deduction_id),"
                  "primary key (role_type, deduction_id))");
    query.exec("create table leave ("
                  "leave_id INTEGER primary key NOT NULL,"
                  "leave_start_date INTEGER NOT NULL,"
                  "leave_end_date INTEGER NOT NULL,"
                  "leave_percentage REAL NOT NULL,"
                  "role_id INTEGER NOT NULL references role(role_id))");

    query.exec("create table variable ("
                  "var_name TEXT primary key NOT NULL,"
                  "var_val INTEGER NOT NULL)");
    query.exec("insert into variable (var_name, var_val)"
                  "values ('built_entry', 1)");
    query.exec("insert into variable (var_name, var_val)"
                  "values ('emp_num', 1)");
    query.exec("insert into variable (var_name, var_val)"
                  "values ('role_num', 1)");
    query.exec("insert into variable (var_name, var_val)"
                  "values ('leave_num', 1)");
    query.exec("insert into variable (var_name, var_val)"
                  "values ('deduc_num', 1)");

    return(QSqlDatabase::database().commit());
}

bool DatabaseControl::dropDatabase()
{
    QSqlDatabase::database().transaction();
    QSqlQuery query;

    query.exec("DROP TABLE user");
    query.exec("DROP TABLE payroll_specialist");
    query.exec("DROP TABLE employee");
    query.exec("DROP TABLE raise");
    query.exec("DROP TABLE role");
    query.exec("DROP TABLE deduction");
    query.exec("DROP TABLE has_deduction");
    query.exec("DROP TABLE leave");
    query.exec("DROP TABLE variable");

    return(QSqlDatabase::database().commit());
}

void DatabaseControl::fillDatabase()
{
    Deduction* ded = new Deduction(getNextDeductionNumber(), "Canada Pension Plan", true, 0.0495);
    addDeduction(*ded);
    addDeductionToRole(*ded, EmployeeRole::DURATION_CONT, EmployeeRole::TIME_FULL, EmployeeRole::TYPE_STAFF);
    addDeductionToRole(*ded, EmployeeRole::DURATION_TERM, EmployeeRole::TIME_FULL, EmployeeRole::TYPE_STAFF);
    addDeductionToRole(*ded, EmployeeRole::DURATION_CONT, EmployeeRole::TIME_PART, EmployeeRole::TYPE_STAFF);
    addDeductionToRole(*ded, EmployeeRole::DURATION_TERM, EmployeeRole::TIME_PART, EmployeeRole::TYPE_STAFF);
    addDeductionToRole(*ded, EmployeeRole::DURATION_CONT, EmployeeRole::TIME_FULL, EmployeeRole::TYPE_FACULTY);
    addDeductionToRole(*ded, EmployeeRole::DURATION_TERM, EmployeeRole::TIME_FULL, EmployeeRole::TYPE_FACULTY);
    addDeductionToRole(*ded, EmployeeRole::DURATION_TERM, EmployeeRole::TIME_PART, EmployeeRole::TYPE_RA);
    addDeductionToRole(*ded, EmployeeRole::DURATION_TERM, EmployeeRole::TIME_PART, EmployeeRole::TYPE_TA);
    delete ded;

    ded = new Deduction(getNextDeductionNumber(), "Employement Insurance", true, 0.0188);
    addDeduction(*ded);
    addDeductionToRole(*ded, EmployeeRole::DURATION_CONT, EmployeeRole::TIME_FULL, EmployeeRole::TYPE_STAFF);
    addDeductionToRole(*ded, EmployeeRole::DURATION_TERM, EmployeeRole::TIME_FULL, EmployeeRole::TYPE_STAFF);
    addDeductionToRole(*ded, EmployeeRole::DURATION_CONT, EmployeeRole::TIME_PART, EmployeeRole::TYPE_STAFF);
    addDeductionToRole(*ded, EmployeeRole::DURATION_TERM, EmployeeRole::TIME_PART, EmployeeRole::TYPE_STAFF);
    addDeductionToRole(*ded, EmployeeRole::DURATION_CONT, EmployeeRole::TIME_FULL, EmployeeRole::TYPE_FACULTY);
    addDeductionToRole(*ded, EmployeeRole::DURATION_TERM, EmployeeRole::TIME_FULL, EmployeeRole::TYPE_FACULTY);
    addDeductionToRole(*ded, EmployeeRole::DURATION_TERM, EmployeeRole::TIME_PART, EmployeeRole::TYPE_RA);
    addDeductionToRole(*ded, EmployeeRole::DURATION_TERM, EmployeeRole::TIME_PART, EmployeeRole::TYPE_TA);
    delete ded;

    ded = new Deduction(getNextDeductionNumber(), "CUPE 4600", true, 0.02);
    addDeduction(*ded);
    addDeductionToRole(*ded, EmployeeRole::DURATION_TERM, EmployeeRole::TIME_PART, EmployeeRole::TYPE_RA);
    addDeductionToRole(*ded, EmployeeRole::DURATION_TERM, EmployeeRole::TIME_PART, EmployeeRole::TYPE_TA);
    delete ded;

    Employee* emp = new Employee("emercer", getNextEmployeeNumber(), "Emily", "Mercer", new Address("Canada", "Ontario", "Ottawa", "Eiffel Ave.", "1024B", "K2C 5J0"));
    TermRole* trole = new TermRole(getNextRoleNumber(), "COMP 3000 TA", 350.25,
                                      new Date(1, 1, 2016), new Date(30, 4, 2016), EmployeeRole::TYPE_TA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployee(*emp);
    addRole(*emp, *trole);
    delete trole;
    addRole(*emp, *(trole = new TermRole(getNextRoleNumber(), "COMP 1005 TA", 700.50,
                           new Date(1, 5, 2016), new Date(30, 6, 2016), EmployeeRole::TYPE_TA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART)));
    delete trole;
    addRole(*emp, *(trole = new TermRole(getNextRoleNumber(), "COMP 1006 TA", 700.50,
                           new Date(1, 7, 2016), new Date(31, 8, 2016), EmployeeRole::TYPE_TA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART)));
    delete trole;
    addRole(*emp, *(trole = new TermRole(getNextRoleNumber(), "COMP 1005 TA", 350.25,
                           new Date(1, 9, 2016), new Date(31, 12, 2016), EmployeeRole::TYPE_TA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART)));
    delete trole;
    addRole(*emp, *(trole = new TermRole(getNextRoleNumber(), "COMP 3000 TA", 350.25,
                           new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_TA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART)));
    delete trole;
    delete emp;

    emp = new Employee("cjohnson", getNextEmployeeNumber(), "Carl", "Johnson", new Address("USA", "New York", "New York", "Wall Street", "1000000", "ABC 123"));
    addEmployee(*emp);
    EmployeeRole* crole = new ContinuingRole(getNextRoleNumber(), "Professor", 123456.78,
                                           new Date(1, 1, 2002), EmployeeRole::TYPE_FACULTY | EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL);
    addRole(*emp, *(ContinuingRole*)crole);
    LeavePeriod* lp;
    addLeavePeriod(*crole, *(lp = new LeavePeriod(getNextLeaveNumber(), new Date(1, 1, 2003), new Date(30, 4, 2003), 0.5)));
    delete lp;
    addLeavePeriod(*crole, *(lp = new LeavePeriod(getNextLeaveNumber(), new Date(1, 1, 2006), new Date(30, 4, 2006), 0.6666)));
    delete lp;
    addLeavePeriod(*crole, *(lp = new LeavePeriod(getNextLeaveNumber(), new Date(1, 1, 2009), new Date(30, 4, 2009), 0.3333)));
    delete lp;
    addLeavePeriod(*crole, *(lp = new LeavePeriod(getNextLeaveNumber(), new Date(1, 1, 2012), new Date(30, 4, 2012), 0.75)));
    delete lp;
    addLeavePeriod(*crole, *(lp = new LeavePeriod(getNextLeaveNumber(), new Date(1, 1, 2016), new Date(30, 4, 2016), 0.25)));
    delete lp;
    delete emp;
    delete crole;

    emp = new Employee("boblawblaw", getNextEmployeeNumber(), "Bob", "BlawBlaw", new Address("Canada", "Ontario", "Ottawa", "Elgin Street", "12A", "A2C 5K7"));
    addEmployee(*emp);

    delete emp;

}

quint32 DatabaseControl::getVariable(QString var_name)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM variable WHERE var_name = (:name)");
    query.bindValue(":name", var_name);
    query.exec();
    query.next();
    int index = query.record().indexOf("var_val");
    return (query.value(index).toInt());
}


void DatabaseControl::setVariable(QString var_name, quint32 value)
{
    QSqlQuery query;
    query.prepare("UPDATE variable SET var_val = (:value) WHERE var_name = (:name)");
    query.bindValue(":name", var_name);
    query.bindValue(":value", value);
    query.exec();
}

#ifdef DEBUG_DB
void DatabaseControl::printDB()
{
    QSqlQuery get;

    std::cout << "user table" << std::endl;
    get.prepare("SELECT * FROM user");
    get.exec();
    while (get.next()) {
        QString userName = get.value(0).toString();
        std::cout << userName.toStdString() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "payroll_specialist table" << std::endl;
    get.prepare("SELECT * FROM payroll_specialist");
    get.exec();
    while (get.next()) {
        QString payroll_specialist = get.value(0).toString();
        std::cout << payroll_specialist.toStdString() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "employee table" << std::endl;
    get.prepare("SELECT * FROM employee");
    get.exec();
    while (get.next()) {
        std::cout << get.value(0).toUInt() << ", "
                  << get.value(1).toString().toStdString() << ", "
                  << get.value(2).toString().toStdString() << ", "
                  << get.value(3).toString().toStdString() << ", "
                  << get.value(4).toString().toStdString() << ", "
                  << get.value(5).toString().toStdString() << ", "
                  << get.value(6).toString().toStdString() << ", "
                  << get.value(7).toString().toStdString() << ", "
                  << get.value(8).toString().toStdString() << ", "
                  << get.value(9).toString().toStdString() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "raise table" << std::endl;
    get.prepare("SELECT * FROM raise");
    get.exec();
    while (get.next()) {
        std::cout << get.value(0).toUInt() << ", "
                  << get.value(1).toUInt() << ", "
                  << get.value(2).toUInt() << ", "
                  << get.value(3).toDouble() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "role table" << std::endl;
    get.prepare("SELECT * FROM role");
    get.exec();
    while (get.next()) {
        std::cout << get.value(0).toUInt() << ", "
                  << get.value(1).toUInt() << ", "
                  << get.value(2).toUInt() << ", "
                  << get.value(3).toDouble() << ", "
                  << get.value(4).toUInt() << ", "
                  << get.value(5).toString().toStdString() << ", "
                  << get.value(6).toUInt() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "deduction table" << std::endl;
    get.prepare("SELECT * FROM deduction");
    get.exec();
    while (get.next()) {
        std::cout << get.value(0).toUInt() << ", "
                  << get.value(1).toString().toStdString() << ", "
                  << get.value(2).toUInt() << ", "
                  << get.value(3).toDouble() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "has_deduction table" << std::endl;
    get.prepare("SELECT * FROM has_deduction");
    get.exec();
    while (get.next()) {
        std::cout << get.value(0).toUInt() << ", "
                  << get.value(1).toUInt() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "leave table" << std::endl;
    get.prepare("SELECT * FROM leave");
    get.exec();
    while (get.next()) {
        std::cout << get.value(0).toUInt() << ", "
                  << get.value(1).toUInt() << ", "
                  << get.value(2).toUInt() << ", "
                  << get.value(3).toDouble() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "variable table" << std::endl;
    get.prepare("SELECT * FROM variable");
    get.exec();
    while (get.next()) {
        std::cout << get.value(0).toString().toStdString() << ", "
                  << get.value(1).toInt() << std::endl;
    }
    std::cout << std::endl;
}
#endif
