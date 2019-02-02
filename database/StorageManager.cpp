#include "StorageManager.h"


#include "memory/TermRole.h"

#include <QtSql>
#include <QSqlQuery>
#include <QListIterator>

#include <fstream>

StorageManager::StorageManager()
{
    dropDatabase();
    buildDatabase();
    fillDatabase();
    std::ifstream db("./cuNICS.db");
    if (!db.good()) {
        qDebug() << "BUILDING...";
        buildDatabase();
        fillDatabase();
    }
    qDebug() << "BUILT";
}

StorageManager::~StorageManager()
{

}

bool StorageManager::addDeduction(quint32 rtype, Deduction& d)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM deduction WHERE deduction_id = :did");
    query.bindValue(":did", d.getDeductionID());

    if (!query.exec()) {
        qDebug() << "ERR: addDeduction" << query.lastError();
        return (false);
    }

    if (!query.next()) {
        query.prepare("INSERT INTO deduction(deduction_id, deduction_name, deduction_fixed_or_percent, deduction_amount) "
                                  "VALUES (:did, :dname, :percent, :amount)");

        query.bindValue(":did", d.getDeductionID());
        query.bindValue(":dname", d.getDeductionName());
        query.bindValue(":percent", d.isPercentage() ? 1 : 0);
        query.bindValue(":amount", d.getDeductionAmount());

        if (!query.exec()) {
            qDebug() << "ERR: addDeduction" << query.lastError();
            return (false);
        }
    }

    query.prepare("INSERT INTO has_deduction(role_type, deduction_id) "
                              "VALUES (:rtype, :did)");

    query.bindValue(":rtype", rtype);
    query.bindValue(":did", d.getDeductionID());

    if (!query.exec()) {
        qDebug() << "ERR: addDeduction" << query.lastError();
        return (false);
    }

    return (true);
}

bool StorageManager::addEmployee(Employee& e)
{
    QSqlQuery query;

    /*
     * prepare statement for adding an employee, and each of their roles to the db
     */

    query.prepare("INSERT INTO user (user_id) VALUES (:user)");
    query.bindValue(":user", e.getUserID());

    if (!query.exec()) {
         qDebug() << "ERR: addEmployee(User)" << e.getUserID() << " : " << query.lastError();
         return (false);
    }

    query.prepare("INSERT INTO employee (employee_id, employee_given_name, employee_family_name, "
                                        "employee_address_number, employee_address_street, "
                                        "employee_address_postal, employee_address_city, "
                                        "employee_address_province, employee_address_country, user_id) "
                                "VALUES (:id, :gname, :fname, "
                                        ":addnum, :addstr, "
                                        ":addpost, :addcity, "
                                        ":addprov, :addcont, :user)");

    query.bindValue(":id", e.getEmployeeID());
    query.bindValue(":gname", e.getGivenName());
    query.bindValue(":fname", e.getFamilyName());
    query.bindValue(":addnum", e.getAddress().getHouseNumber());
    query.bindValue(":addstr", e.getAddress().getStreet());
    query.bindValue(":addpost", e.getAddress().getPostalCode());
    query.bindValue(":addcity", e.getAddress().getCity());
    query.bindValue(":addprov", e.getAddress().getProvince());
    query.bindValue(":addcont", e.getAddress().getCountry());
    query.bindValue(":user", e.getUserID());

    if (!query.exec()) {
        qDebug() << "ERR: addEmployee(Employee)" << e.getGivenName();
        return (false);
    }

    setVariable("emp_num", e.getEmployeeID() + 1);

    return (true);
}

bool StorageManager::addEmployeeRole(quint32 eid, EmployeeRole& er)
{
    QSqlQuery query;
    query.prepare("INSERT INTO role (role_id, role_start_date, role_end_date, role_salary, role_type, role_name, employee_id) "
                              "VALUES (:rid, :sdate, :edate, :salary, :rtype, :rname, :eid)");

    query.bindValue(":rid", er.getRoleID());
    query.bindValue(":sdate", er.getStartDate().toInteger());
    query.bindValue(":edate", (er.isTermRole()) ? ((TermRole*)&er)->getEndDate().toInteger() : 0);
    query.bindValue(":salary", er.getSalary());
    query.bindValue(":rtype", er.getType());
    query.bindValue(":rname", er.getRoleName());
    query.bindValue(":eid", eid);

    if (!query.exec()) {
        qDebug() << "ERR: addRole(Term)" << query.lastError();
        return (false);
    }

    setVariable("role_num", er.getRoleID() + 1);

    return(true);
}

bool StorageManager::addLeavePeriod(ContinuingRole& cr, LeavePeriod& lp)
{
    QSqlQuery query;
    query.prepare("INSERT INTO leave (leave_id, leave_start_date, leave_end_date, leave_percentage, role_id) "
                              "VALUES (:lid, :sdate, :edate, :lper, :rid)");

    query.bindValue(":lid", lp.getLeaveID());
    query.bindValue(":sdate", lp.getStartDate().toInteger());
    query.bindValue(":edate", lp.getEndDate().toInteger());
    query.bindValue(":lper", lp.getPercent());
    query.bindValue(":rid", cr.getRoleID());

    if (!query.exec()) {
        qDebug() << "ERR: addLeavePeriod" << query.lastError();
        return (false);
    }

    setVariable("leave_num", lp.getLeaveID() + 1);

    return (true);
}


bool StorageManager::applyRaise(Raise& raise)
{
    QSqlQuery query;
    query.prepare("INSERT INTO raise (raise_date, role_type, raise_fixed_or_percent, raise_amount) "
                  "            VALUES (:date, :rtype, :percent, :amount)");

    query.bindValue(":date", raise.getApplyDate().toInteger());
    query.bindValue(":rtype", raise.getRole());
    query.bindValue(":percent", (quint32)raise.isPercent());
    query.bindValue(":amount", raise.getAmount());

    if (!query.exec()) {
        qDebug() << "ERR: applyRaise" << query.lastError();
        return (false);
    }

    return (true);
}

bool StorageManager::getEmployees(QMap<quint32, Employee*>* employees)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM employee");

    if (!query.exec()) {
        qDebug() << "ERR: getEmployees" << query.lastError();
        return (false);
    }

    while (query.next()) {
        quint32 eid = query.value(0).toUInt();
        QString uid = query.value(1).toString();
        QString gname = query.value(2).toString();
        QString fname = query.value(3).toString();
        QString add_num = query.value(4).toString();
        QString add_str = query.value(5).toString();
        QString add_post = query.value(6).toString();
        QString add_city = query.value(7).toString();
        QString add_prov = query.value(8).toString();
        QString add_cont = query.value(9).toString();

        Employee* emp = new Employee(uid, eid, gname, fname, new Address(add_cont, add_prov, add_city, add_str, add_num, add_post));

        employees->insert(eid, emp);
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

        employees->value(eid)->addEmployeeRole(new_role);
    }



    return (true);
}

bool StorageManager::updateEmployee(Employee& e)
{
    QSqlQuery query;
    query.prepare("UPDATE employee "
                  "SET employee_given_name = :gname, employee_family_name = :fname, "
                      "employee_address_number = :addnum, employee_address_street = :addstr, "
                      "employee_address_postal = :addpost, employee_address_city = :addcity, "
                      "employee_address_province = :addprov, employee_address_country = :addcont "
                  "WHERE employee_id = :eid");

    query.bindValue(":eid", e.getEmployeeID());
    query.bindValue(":gname", e.getGivenName());
    query.bindValue(":fname", e.getFamilyName());
    query.bindValue(":addnum", e.getAddress().getHouseNumber());
    query.bindValue(":addstr", e.getAddress().getStreet());
    query.bindValue(":addpost", e.getAddress().getPostalCode());
    query.bindValue(":addcity", e.getAddress().getCity());
    query.bindValue(":addprov", e.getAddress().getProvince());
    query.bindValue(":addcont", e.getAddress().getCountry());
    if (!query.exec()) {
        qDebug() << "ERR: updateEmployee" << query.lastError();
        return (false);
    }

    return (true);
}

bool StorageManager::updateEmployeeAttributes(quint32 eid, QList<AttributePair*>* attributes)
{
    QSqlQuery query;

    QString queryString = "UPDATE employee SET ";

    QListIterator<AttributePair*> it(*attributes);
    bool first = true;

    while (it.hasNext()) {
        AttributePair* ap = it.next();

        queryString = queryString % (first ? "" : ", ");
        first = false;

        switch (ap->getAttributeID()) {
            case GIVEN_NAME:
                queryString = queryString % "employee_given_name = :gname";
                break;
            case FAMILY_NAME:
                queryString = queryString % "employee_family_name = :fname";
                break;
            case STREET_NUMBER:
                queryString = queryString % "employee_address_number = :addnum";
                break;
            case STREET_ADDRESS:
                queryString = queryString % "employee_address_street = :addstr";
                break;
            case POSTAL_CODE:
                queryString = queryString % "employee_address_postal = :addpost";
                break;
            case CITY:
                queryString = queryString % "employee_address_city = :addcity";
                break;
            case PROVINCE:
                queryString = queryString % "employee_address_province = :addprov";
                break;
            case COUNTRY:
                queryString = queryString % "employee_address_country = :addcont";
                break;
            default:
                break;
        }
    }

    queryString = queryString % " WHERE employee_id = :eid";

    query.prepare(queryString);

    it = QListIterator<AttributePair*>(*attributes);
    while (it.hasNext()) {
        AttributePair* ap = it.next();

        switch (ap->getAttributeID()) {
            case GIVEN_NAME:
                query.bindValue(":gname", ap->getValue());
                break;
            case FAMILY_NAME:
                query.bindValue(":fname", ap->getValue());
                break;
            case STREET_NUMBER:
                query.bindValue(":addnum", ap->getValue());
                break;
            case STREET_ADDRESS:
                query.bindValue(":addstr", ap->getValue());
                break;
            case POSTAL_CODE:
                query.bindValue(":addpost", ap->getValue());
                break;
            case CITY:
                query.bindValue(":addcity", ap->getValue());
                break;
            case PROVINCE:
                query.bindValue(":addprov", ap->getValue());
                break;
            case COUNTRY:
                query.bindValue(":addcont", ap->getValue());
                break;
            default:
                break;
        }
    }
    query.bindValue(":eid", eid);

    if (!query.exec()) {
        qDebug() << "ERR: updateEmployeeAttributes" << query.lastError();
        return (false);
    }

    return (true);
}

bool StorageManager::updateEmployeeRole(EmployeeRole& er)
{
    QSqlQuery query;
    query.prepare("UPDATE role "
                  "SET role_salary = :salary, role_name = :name "
                  "WHERE role_id = :rid");

    query.bindValue(":salary", er.getSalary());
    query.bindValue(":name", er.getRoleName());
    query.bindValue(":rid", er.getRoleID());

    if (!query.exec()) {
        qDebug() << "ERR: updateEmployeeRole" << query.lastError();
        return (false);
    }

    return (true);
}

quint32 StorageManager::getNextEmployeeNumber()
{
    quint32 next = getVariable("emp_num");
    setVariable("emp_num", next + 1);
    return (next);
}

quint32 StorageManager::getNextRoleNumber()
{
    quint32 next = getVariable("role_num");
    setVariable("role_num", next + 1);
    return (next);
}

quint32 StorageManager::getNextLeaveNumber()
{
    quint32 next = getVariable("leave_num");
    setVariable("leave_num", next + 1);
    return (next);
}

quint32 StorageManager::getNextDeductionNumber()
{
    quint32 next = getVariable("deduc_num");
    setVariable("deduc_num", next + 1);
    return (next);
}

quint32 StorageManager::getVariable(QString var_name)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM variable WHERE var_name = (:name)");
    query.bindValue(":name", var_name);
    query.exec();
    query.next();
    int index = query.record().indexOf("var_val");
    return (query.value(index).toInt());
}


void StorageManager::setVariable(QString var_name, quint32 value)
{
    QSqlQuery query;
    query.prepare("UPDATE variable SET var_val = (:value) WHERE var_name = (:name)");
    query.bindValue(":name", var_name);
    query.bindValue(":value", value);
    query.exec();
}

bool StorageManager::buildDatabase()
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

bool StorageManager::dropDatabase()
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

bool StorageManager::fillDatabase()
{
    Deduction* ded = new Deduction(1, "Canada Pension Plan", true, 0.0495);
    addDeduction(EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_STAFF, *ded);
    addDeduction(EmployeeRole::DURATION_TERM | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_STAFF, *ded);
    addDeduction(EmployeeRole::DURATION_CONT | EmployeeRole::TIME_PART | EmployeeRole::TYPE_STAFF, *ded);
    addDeduction(EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART | EmployeeRole::TYPE_STAFF, *ded);
    addDeduction(EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_FACULTY, *ded);
    addDeduction(EmployeeRole::DURATION_TERM | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_FACULTY, *ded);
    addDeduction(EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART | EmployeeRole::TYPE_RA, *ded);
    addDeduction(EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART | EmployeeRole::TYPE_TA, *ded);
    delete ded;

    ded = new Deduction(2, "Employement Insurance", true, 0.0188);
    addDeduction(EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_STAFF, *ded);
    addDeduction(EmployeeRole::DURATION_TERM | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_STAFF, *ded);
    addDeduction(EmployeeRole::DURATION_CONT | EmployeeRole::TIME_PART | EmployeeRole::TYPE_STAFF, *ded);
    addDeduction(EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART | EmployeeRole::TYPE_STAFF, *ded);
    addDeduction(EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_FACULTY, *ded);
    addDeduction(EmployeeRole::DURATION_TERM | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_FACULTY, *ded);
    addDeduction(EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART | EmployeeRole::TYPE_RA, *ded);
    addDeduction(EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART | EmployeeRole::TYPE_TA, *ded);
    delete ded;

    ded = new Deduction(3, "CUPE 4600", true, 0.02);
    addDeduction(EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART | EmployeeRole::TYPE_RA, *ded);
    addDeduction(EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART | EmployeeRole::TYPE_TA, *ded);
    delete ded;

    Employee* emp = new Employee("emercer", 1, "Emily", "Mercer", new Address("Canada", "Ontario", "Ottawa", "Eiffel Ave.", "1024B", "K2C 5J0"));
    TermRole* trole = new TermRole(1, "COMP 3000 TA", 350.25,
                                      new Date(1, 1, 2016), new Date(30, 4, 2016), EmployeeRole::TYPE_TA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployee(*emp);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete trole;
    addEmployeeRole(emp->getEmployeeID(), *(trole = new TermRole(2, "COMP 1005 TA", 700.50,
                           new Date(1, 5, 2016), new Date(30, 6, 2016), EmployeeRole::TYPE_TA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART)));
    delete trole;
    addEmployeeRole(emp->getEmployeeID(), *(trole = new TermRole(3, "COMP 1006 TA", 700.50,
                           new Date(1, 7, 2016), new Date(31, 8, 2016), EmployeeRole::TYPE_TA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART)));
    delete trole;
    addEmployeeRole(emp->getEmployeeID(), *(trole = new TermRole(4, "COMP 1005 TA", 350.25,
                           new Date(1, 9, 2016), new Date(31, 12, 2016), EmployeeRole::TYPE_TA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART)));
    delete trole;
    addEmployeeRole(emp->getEmployeeID(), *(trole = new TermRole(5, "COMP 3000 TA", 350.25,
                           new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_TA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART)));
    delete trole;
    delete emp;

    emp = new Employee("cjohnson", 2, "Carl", "Johnson", new Address("USA", "San Andreas", "Los Santos", "Grove Street", "1000000", "ABC 123"));
    addEmployee(*emp);
    ContinuingRole* crole = new ContinuingRole(6, "Professor", 123456.78,
                                           new Date(1, 1, 2002), EmployeeRole::TYPE_FACULTY | EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL);
    addEmployeeRole(emp->getEmployeeID(), *(ContinuingRole*)crole);
    LeavePeriod* lp;
    addLeavePeriod(*crole, *(lp = new LeavePeriod(1, new Date(1, 1, 2003), new Date(30, 4, 2003), 50)));
    delete lp;
    addLeavePeriod(*crole, *(lp = new LeavePeriod(2, new Date(1, 1, 2006), new Date(30, 4, 2006), 66)));
    delete lp;
    addLeavePeriod(*crole, *(lp = new LeavePeriod(3, new Date(1, 1, 2009), new Date(30, 4, 2009), 33)));
    delete lp;
    addLeavePeriod(*crole, *(lp = new LeavePeriod(4, new Date(1, 1, 2012), new Date(30, 4, 2012), 75)));
    delete lp;
    addLeavePeriod(*crole, *(lp = new LeavePeriod(5, new Date(1, 1, 2016), new Date(30, 4, 2016), 25)));
    delete lp;
    delete emp;
    delete crole;

    emp = new Employee("bobloblaw", 3, "Bob", "Loblaw", new Address("Canada", "Ontario", "Ottawa", "Elgin Street", "12A", "A2C 5K7"));
    addEmployee(*emp);
    trole = new TermRole(6, "Computer Science Research Assistant", 555.25,
                         new Date(1, 1, 2016), new Date(30, 4, 2016), EmployeeRole::TYPE_RA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete emp;
    delete trole;

    emp = new Employee("blawblaw", 4, "Bob", "LawBlaw", new Address("Canada", "Ontario", "Ottawa", "Elgin Street", "12A", "A2C 5K7"));
    addEmployee(*emp);
    trole = new TermRole(7, "Cook", 256.25,
                         new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_STAFF | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete emp;
    delete trole;

    emp = new Employee("dannyh", 5, "Danny", "O'Homer", new Address("Canada", "Ontario", "Ottawa", "Bay Street", "200", "A2C 5K7"));
    addEmployee(*emp);
    crole = new ContinuingRole(8, "Master Chef", 123456.78,
                               new Date(1, 1, 2007), EmployeeRole::TYPE_STAFF | EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL);
    addEmployeeRole(emp->getEmployeeID(), *crole);
    delete crole;
    delete emp;

    emp = new Employee("timmyt", 6, "Timmy", "Tortoise", new Address("Canada", "Ontario", "Ottawa", "River Avenue", "1024", "K2C 0K7"));
    addEmployee(*emp);
    trole = new TermRole(9, "Cook", 256.25,
                         new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_STAFF | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete trole;
    delete emp;

    emp = new Employee("msvd", 7, "Michael", "van Dyk", new Address("Canada", "Ontario", "Ottawa", "Eiffel Avenue", "193", "A2C 5K7"));
    addEmployee(*emp);
    crole = new ContinuingRole(10, "Master Chef", 123456.78,
                               new Date(1, 1, 2007), EmployeeRole::TYPE_STAFF | EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL);
    addEmployeeRole(emp->getEmployeeID(), *crole);
    delete crole;
    delete emp;

    emp = new Employee("johncena", 8, "John", "Cena", new Address("Canada", "Ontario", "Ottawa", "Invisible Court", "1", "U5T 9E3"));
    addEmployee(*emp);
    crole = new ContinuingRole(11, "Master Chef", 123456.78,
                               new Date(4, 4, 2015), EmployeeRole::TYPE_STAFF | EmployeeRole::DURATION_CONT | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *crole);
    delete crole;
    delete emp;

    emp = new Employee("undertaker", 9, "Under", "Taker", new Address("Canada", "Ontario", "Ottawa", "Cemetary Road", "5B", "R4A 9J8"));
    addEmployee(*emp);
    crole = new ContinuingRole(12, "Professor", 123456.78,
                               new Date(4, 6, 2013), EmployeeRole::TYPE_FACULTY | EmployeeRole::DURATION_CONT | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *crole);
    delete emp;
    delete crole;

    emp = new Employee("therock", 10, "Dwayne", "Johnson", new Address("Canada", "Ontario", "Ottawa", "Bank Street", "450", "K2D 6A1"));
    addEmployee(*emp);
    crole = new ContinuingRole(13, "Professor", 123456.78,
                               new Date(4, 8, 2004), EmployeeRole::TYPE_FACULTY | EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL);

    addEmployeeRole(emp->getEmployeeID(), *crole);
    delete emp;
    delete crole;

    emp = new Employee("jimmywales", 11, "Jimmy", "Wales", new Address("Canada", "Ontario", "Ottawa", "Library Circle", "2", "K2C 7F2"));
    addEmployee(*emp);
    trole = new TermRole(14, "Cook", 256.25,
                         new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_STAFF | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete trole;
    delete emp;

    emp = new Employee("bimmybales", 12, "Bimmy", "Bales", new Address("Canada", "Ontario", "Ottawa", "Donation Street", "1", "K2A 1B4"));
    addEmployee(*emp);
    trole = new TermRole(15, "Cook", 256.25,
                         new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_STAFF | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete trole;
    delete emp;

    emp = new Employee("ablanch", 13, "Alexis", "Blanchard", new Address("Canada", "Ontario", "Ottawa", "Bank Street", "231", "K2D 5C2"));
    addEmployee(*emp);
    trole = new TermRole(16, "Cook", 256.25,
                         new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_STAFF | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete trole;
    delete emp;

    emp = new Employee("grantb13", 14, "Grant", "Barton", new Address("Canada", "Ontario", "Ottawa", "Lyon Avenue", "222", "K2C 7F2"));
    addEmployee(*emp);
    trole = new TermRole(17, "Cook", 256.25,
                         new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_STAFF | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete trole;
    delete emp;

    emp = new Employee("rlewis", 15, "Ronald", "Lewis", new Address("Canada", "Ontario", "Ottawa", "Kent Street", "605A", "B2R 7F2"));
    addEmployee(*emp);
    trole = new TermRole(18, "Computer Science Research Assistant", 555.25,
                         new Date(1, 1, 2016), new Date(30, 4, 2016), EmployeeRole::TYPE_RA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete trole;
    trole = new TermRole(19, "Computer Science Research Assistant", 555.25,
                         new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_RA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete emp;
    delete trole;

    emp = new Employee("blewis", 16, "Becky", "Lewis", new Address("Canada", "Ontario", "Ottawa", "Kent Street", "605A", "B2R 7F2"));
    addEmployee(*emp);
    trole = new TermRole(20, "Rocket Research Assistant", 555.25,
                         new Date(1, 1, 2016), new Date(30, 4, 2016), EmployeeRole::TYPE_RA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete trole;
    trole = new TermRole(21, "Rocket Science Research Assistant", 555.25,
                         new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_RA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete emp;
    delete trole;

    emp = new Employee("glewis", 17, "George", "Lewis", new Address("Canada", "Ontario", "Ottawa", "Kent Street", "605A", "B2R 7F2"));
    addEmployee(*emp);
    trole = new TermRole(22, "HCI Research Assistant", 555.25,
                         new Date(1, 1, 2016), new Date(30, 4, 2016), EmployeeRole::TYPE_RA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete trole;
    trole = new TermRole(23, "HCI Research Assistant", 555.25,
                         new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_RA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete emp;
    delete trole;

    emp = new Employee("hlewis", 18, "Harold", "Lewis", new Address("Canada", "Ontario", "Ottawa", "Kent Street", "605A", "B2R 7F2"));
    addEmployee(*emp);
    trole = new TermRole(24, "HCI Research Assistant", 555.25,
                         new Date(1, 1, 2016), new Date(30, 4, 2016), EmployeeRole::TYPE_RA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete trole;
    trole = new TermRole(25, "HCI Research Assistant", 555.25,
                         new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_RA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete emp;
    delete trole;

    emp = new Employee("stevewilliams", 19, "Steve", "Williams", new Address("Canada", "Ontario", "Ottawa", "Bay Street", "420", "K2C 7F2"));
    addEmployee(*emp);
    trole = new TermRole(26, "Operating Systems Research Assistant", 555.25,
                         new Date(1, 1, 2016), new Date(30, 4, 2016), EmployeeRole::TYPE_RA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete trole;
    trole = new TermRole(27, "Rocket Science Research Assistant", 555.25,
                         new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_RA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete emp;
    delete trole;

    emp = new Employee("thebigboss", 20, "Emperor", "Nero", new Address("Canada", "Ontario", "Ottawa", "Golden Palace Drive", "1", "K2C 7C5"));
    addEmployee(*emp);
    trole = new TermRole(28, "Ancient Rome Research Assistant", 555.25,
                         new Date(1, 1, 2016), new Date(30, 4, 2016), EmployeeRole::TYPE_RA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete trole;
    trole = new TermRole(29, "Tyrrant Research Assistant", 555.25,
                         new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_RA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    trole = new TermRole(30, "Ancient Rome Teaching Assistant", 354.25,
                         new Date(1, 1, 2016), new Date(30, 4, 2016), EmployeeRole::TYPE_TA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete trole;
    trole = new TermRole(31, "Tyrrant Teaching Assistant", 354.25,
                         new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_TA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete emp;
    delete trole;

    emp = new Employee("stevenash", 21, "Steve", "Nash", new Address("Canada", "Ontario", "Ottawa", "Wellington Drive", "124", "K5C 6E3"));
    addEmployee(*emp);
    trole = new TermRole(32, "Security Teaching Assistant", 354.25,
                         new Date(1, 1, 2016), new Date(30, 4, 2016), EmployeeRole::TYPE_TA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete trole;
    trole = new TermRole(33, "Database Teaching Assistant", 354.25,
                         new Date(1, 1, 2017), new Date(30, 4, 2017), EmployeeRole::TYPE_TA | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *trole);
    delete trole;
    delete emp;

    emp = new Employee("davey", 22, "Dave", "Yandle", new Address("Canada", "Ontario", "Ottawa", "Slater Street", "2560", "K2C GT1"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("dimmydales", 23, "Dimmy", "Dales", new Address("Canada", "Ontario", "Ottawa", "Donation Street", "1", "K2A 1B4"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("scienceguy", 24, "Bill", "Nye", new Address("Canada", "Ontario", "Ottawa", "Albert Street", "225", "K2C 8H4"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("ilovecocacola", 25, "Pablo", "Escobar", new Address("Canada", "Ontario", "Ottawa", "Prison Ave", "34A", "R25 8T2"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("danielwhomer", 26, "Daniel", "Homer", new Address("Canada", "Ontario", "Ottawa", "Eiffel Avenue", "977", "K2C 0J3"));
    addEmployee(*emp);
    TermRole* dtrole = new TermRole(34, "Computer Science TA", 1400,
                                           new Date(1, 1, 2002), new Date(1,12,2002), EmployeeRole::TYPE_STAFF | EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART);
    addEmployeeRole(emp->getEmployeeID(), *(TermRole*)dtrole);
    delete emp;
    delete dtrole;

    emp = new Employee("ctrace", 27, "Trace", "Cory", new Address("Canada", "Ontario", "Ottawa", "Kent Street", "2221", "K2C 7F2"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("taylorh", 28, "Taylor", "Holmwood", new Address("Canada", "Ontario", "Ottawa", "Bronson Avenue", "500", "K6J 8E1"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("bertyw", 29, "Bert", "Williams", new Address("Canada", "Ontario", "Ottawa", "Bank Street", "2266", "K2F 9A2"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("hughm", 30, "Hugh", "Mungus", new Address("Canada", "Ontario", "Ottawa", "Grosse Street", "288", "K2C 7H7"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("jgards", 31, "Josh", "Gardiner", new Address("Canada", "Ontario", "Ottawa", "Lyon Avenue", "275", "H4K 952"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("jimmyo", 32, "James", "Oliver", new Address("Canada", "Ontario", "Ottawa", "Bank Street", "800", "K2C 7J1"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("thegreat8", 33, "Alexander", "Ovechkin", new Address("Canada", "Ontario", "Ottawa", "Bank Street", "2", "K2C 7F2"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("sidthekid", 34, "Sidney", "Crosby", new Address("Canada", "Ontario", "Ottawa", "Bank Street", "2", "K2C 7F2"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("fcoleman", 35, "Farren", "Coleman", new Address("Canada", "Ontario", "Ottawa", "Claymor Avenue", "92", "K2C 0J3"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("connorwall", 36, "Connor", "Waller", new Address("Canada", "Ontario", "Ottawa", "Claymor Avenue", "92", "K2C 0J3"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("rodf", 37, "Rod", "Farva", new Address("Canada", "Ontario", "Ottawa", "Trooper Lane", "2", "K2C 7F2"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("robr", 38, "Robert", "Roto", new Address("Canada", "Ontario", "Ottawa", "Trooper Lane", "2", "K2C 7F2"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("thorny", 39, "Arcot", "Ramathorn", new Address("Canada", "Ontario", "Ottawa", "Trooper Lane", "2", "K2C 7F2"));
    addEmployee(*emp);
    delete emp;

    emp = new Employee("mac", 40, "MacIntyre", "Womack", new Address("Canada", "Ontario", "Ottawa", "Trooper Lane", "2", "K2C 7F2"));
    addEmployee(*emp);
    delete emp;


    Raise* raise = new Raise(EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_FACULTY, new Date(1, 12, 2016), true, 25);
    applyRaise(*raise);
    delete raise;


    return (true);
}


#include <iostream>
void printDB()
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

void StorageManager::printDB()
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
