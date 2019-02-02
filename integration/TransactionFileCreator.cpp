#include "TransactionFileCreator.h"

#include "memory/EmployeeRole.h"

/**
 * @brief convertInt32ToChars
 *
 * Converts a 32 bit integer to characters.
 *
 * @param file the file stream to put the characters in
 * @param i32  the integer to convert
 * @return the file stream
 */
std::fstream& convertInt32ToChars(std::fstream& file, quint32 i32);

/**
 * @brief convertDoubleToChars
 *
 * Converts a double to characters.
 *
 * @param file the file stream to put the characters in
 * @param i32  the double to convert
 * @return the file stream
 */
std::fstream& convertDoubleToChars(std::fstream& file, double dbl);

/**
 * @brief TransactionFileCreator::TransactionFileCreator
 *
 * Opens the transaction file to create a transaction file.
 */
TransactionFileCreator::TransactionFileCreator()
{
    file.open("transaction.txt", std::fstream::out);
}

/**
 * @brief TransactionFileCreator::~TransactionFileCreator
 *
 * Adds the end file character and closes the file.
 */
TransactionFileCreator::~TransactionFileCreator()
{
    file << (char)0;
    file.close();
}

/**
 * @brief TransactionFileCreator::makeSampleTransactionFile
 *
 * Makes a sample transaction file to test the integration process. Adds
 * several employess and makes changes to existing employees.
 */
void TransactionFileCreator::makeSampleTransactionFile()
{
    TransactionFileCreator sample;

    sample.addEmployee(12345678, "transfileemp", "Marco", "Polo", "123", "Fake Street", "A1B 2C3", "Fakesville", "Faketario", "Fakada");

    AttributePair marcus[3] = {AttributePair(STREET_ADDRESS, "321"),
                               AttributePair(GIVEN_NAME, "Marcus"),
                               AttributePair(FAMILY_NAME, "Aurelius")};
    sample.updateBasic(12345678, 3, marcus);


    AttributePair danny[4] = {AttributePair(STREET_ADDRESS, "Bay Street"),
                               AttributePair(GIVEN_NAME, "Dan"),
                               AttributePair(FAMILY_NAME, "Homer"),
                               AttributePair(POSTAL_CODE, "K9J 9K3")};
    sample.updateBasic(5,4,danny);

    AttributePair becky[4] = {AttributePair(STREET_ADDRESS, "Kent Street"),
                               AttributePair(STREET_NUMBER, "201"),
                               AttributePair(FAMILY_NAME, "Ferguson"),
                               AttributePair(POSTAL_CODE, "K9J 9K3")};
    sample.updateBasic(16,4,becky);


    sample.updateBasic(17,4,becky);
    sample.updateBasic(18,4,becky);


    Date raise(31, 12, 2016);
    sample.applyRaise(raise, 5, true, 0.01);
    sample.applyRaise(raise, 9, false, 100);

    Date roles(1, 1, 2016);
    Date rolee(1, 12, 2016);
    sample.addEmployeeRole(12345678, "Super Dood", 1234.25, roles, EmployeeRole::DURATION_CONT | EmployeeRole::TIME_PART | EmployeeRole::TYPE_STAFF);
    sample.addEmployeeRole(12345678, "Temp Super Dood", 1234.25, roles, rolee, EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART | EmployeeRole::TYPE_STAFF);
    sample.updatePay(12345678, 0, 12543);

    sample.addEmployeeRole(30, "Philosophy Undergraduate TA", 1500, roles,rolee, EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART | EmployeeRole::TYPE_TA);

    sample.addEmployeeRole(29,"Philosophy Associate Professor", 64000, roles, EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_FACULTY);
    sample.updatePay(29, 0, 65000 );

    Date securityStart(10,6, 2001);
    sample.addEmployeeRole(37, "Security Guard", 48500, securityStart, EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_STAFF);
    sample.addEmployeeRole(38, "Security Guard", 48500, securityStart, EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_STAFF);
    sample.addEmployeeRole(39, "Security Guard", 48500, securityStart, EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_STAFF);
    sample.addEmployeeRole(40, "Security Guard", 48500, securityStart, EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_STAFF);

    sample.addEmployee(41,"je3foster", "Jeff", "Foster", "2","Trooper Lane", "K2C 7F2", "Ottawa", "Ontario", "Canada" );
    sample.addEmployeeRole(41, "Security Chief", 65000, roles, EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_STAFF);

    sample.addLeavePeriod(37,0,roles,rolee,35);

    Date sanitationStart(1,1,2013);
    sample.addEmployee(42,"mbanner", "Matthew", "Banner", "65", "Bronson Avenue", "K7C 9N1", "Ottawa", "Ontario", "Canada");
    sample.addEmployeeRole(42,"Janitor", 38500, sanitationStart, EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_STAFF);

    sample.addEmployee(43,"jsmith", "Jennifer", "Smith", "87", "Lovely Lane", "T8A 8A3", "Gatineau", "Quebec", "Canada");
    sample.addEmployeeRole(43,"Janitor", 38500, sanitationStart, EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_STAFF);

    sample.addEmployee(44,"fbackstrom", "Frank", "Backstrom", "9001", "Main Street", "G3A 9K1", "Kanata", "Ontario", "Canada");
    sample.addEmployeeRole(44, "Janitor", 38500, sanitationStart, EmployeeRole::DURATION_CONT | EmployeeRole::TIME_FULL | EmployeeRole::TYPE_STAFF);


    Date start(1,1,2017);
    Date end(1,4,2017);
    sample.addLeavePeriod(12345678, 0, start, end, 75);

    sample.addEmployeeRole(12,"Cafeteria Cashier", 24000, start,end, EmployeeRole::DURATION_TERM | EmployeeRole::TIME_PART | EmployeeRole::TYPE_STAFF);

    AttributePair dave[4] = {AttributePair(STREET_ADDRESS, "Main Street"),
                               AttributePair(STREET_NUMBER, "2031"),
                               AttributePair(CITY, "Kanata"),
                               AttributePair(POSTAL_CODE, "G3L 8N2")};
    sample.updateBasic(22,4,dave);
}

/**
 * @brief TransactionFileCreator::addEmployee
 *
 *  Adds an employee transaction to the transaction file being created.
 *
 * @param emp_num     the employee number of the new employee
 * @param login_name  the login name of the new employee
 * @param given_name  the given name of the new employee
 * @param family_name the family name of the new employee
 * @param house_num   the house number fo the new employee
 * @param street_name the street name of the new employee
 * @param postal_code the postal code of the new employee
 * @param city        the city of the new employee
 * @param province    the province of the new employee
 * @param country     the country of the new employee
 */
void TransactionFileCreator::addEmployee(quint32 emp_num, QString login_name, QString given_name,
                 QString family_name, QString house_num, QString street_name,
                 QString postal_code, QString city, QString province, QString country)
{
    file << (char)1;
    convertInt32ToChars(file, emp_num) << login_name.toStdString() << (char)0
         << given_name.toStdString() << (char)0 << family_name.toStdString() << (char)0 << house_num.toStdString()
         << (char)0 << street_name.toStdString() << (char)0 << postal_code.toStdString() << (char)0 << city.toStdString()
         << (char)0 << province.toStdString() << (char)0 << country.toStdString() << (char)0;
}

/**
 * @brief TransactionFileCreator::updateBasic
 *
 * Adds a basic update transaction to the transaction file.
 *
 * @param emp_num    the employee number of teh employee to update
 * @param attr_count the number of attributes to update
 * @param attributes the attribute IDs and update values
 */
void TransactionFileCreator::updateBasic(quint32 emp_num, quint32 attr_count, AttributePair* attributes)
{
    file << (char)2;
    convertInt32ToChars(file, emp_num);

    for (quint32 i=0; i<attr_count; ++i) {
        file << attributes[i].idAsChar() << attributes[i].getValue().toStdString() << (char)0;
    }

    file << (char)0;
}

/**
 * @brief TransactionFileCreator::updatePay
 *
 * Updates the pay of an employee role.
 *
 * @param emp_num  the employee number of the employee to update
 * @param role_num the position of the role in the employee's role array
 * @param amount   the amount to change the wage to
 */
void TransactionFileCreator::updatePay(quint32 emp_num, quint8 role_num, double amount)
{
    file << (char)3;
    convertInt32ToChars(file, emp_num) << (char)role_num;
    convertDoubleToChars(file, amount);
}

/**
 * @brief TransactionFileCreator::applyRaise
 *
 * Adds a apply raise transaction to the transaction file.
 *
 * @param apply_date the date to update the transaction
 * @param role_type  the role type to apply the raise to
 * @param percent    if the raise is percent based or a fixed amount
 * @param amount     the amount or percent to increase the pay by
 */
void TransactionFileCreator::applyRaise(Date& apply_date, quint8 role_type, bool percent, double amount)
{
    file << (char)4;
    convertInt32ToChars(file, apply_date.toInteger()) << (char)role_type << (char)percent;
    convertDoubleToChars(file, amount);
}

/**
 * @brief TransactionFileCreator::addLeavePeriod
 *
 * Adds a leave period transaction to the transaction file.
 *
 * @param emp_num  the employee number to add the leave period to
 * @param role_num the index of the role in the employee's role list to add the leave period to
 * @param start    the start date of the leave
 * @param end      the end date of the leave
 * @param percent  the percentage to reduce the pay by
 */
void TransactionFileCreator::addLeavePeriod(quint32 emp_num, quint8 role_num, Date& start, Date& end, double percent)
{
    file << (char)5;
    convertInt32ToChars(file, emp_num) << (char)role_num;
    convertInt32ToChars(file, start.toInteger());
    convertInt32ToChars(file, end.toInteger());
    convertDoubleToChars(file, percent);
}

/**
 * @brief TransactionFileCreator::addEmployeeRole
 *
 * Add an add employee role transaction to the transaction file.
 *
 * @param emp_num    the employee number of teh employee to add to
 * @param roleName   the title of the role
 * @param roleSalary the role salary
 * @param startDate  the start date of the employee role
 * @param roleType   the type of role
 */
void TransactionFileCreator::addEmployeeRole(quint32 emp_num, QString roleName, double roleSalary, Date& startDate, quint8 roleType)
{
    Date empty(0, 0, 0);
    addEmployeeRole(emp_num, roleName, roleSalary, startDate, empty, roleType);
}

/**
 * @brief TransactionFileCreator::addEmployeeRole
 *
 * Add an add employee role transaction to the transaction file.
 *
 * @param emp_num    the employee number of teh employee to add to
 * @param roleName   the title of the role
 * @param roleSalary the role salary
 * @param startDate  the start date of the employee role
 * @param endDate    the end date of teh employee role
 * @param roleType   the type of role
 */
void TransactionFileCreator::addEmployeeRole(quint32 emp_num, QString roleName, double roleSalary, Date& startDate, Date& endDate, quint8 roleType)
{
    file << (char)6;
    convertInt32ToChars(file, emp_num) << roleName.toStdString() << (char)0;
    convertDoubleToChars(file, roleSalary);
    convertInt32ToChars(file, startDate.toInteger());
    convertInt32ToChars(file, endDate.toInteger()) << (char)roleType;
}

/**
 * @brief convertInt32ToChars
 *
 * Converts a 32 bit integer to characters.
 *
 * @param file the file stream to put the characters in
 * @param i32  the integer to convert
 * @return the file stream
 */
std::fstream& convertInt32ToChars(std::fstream& file, quint32 i32)
{
    cInt8to32 helper;
    helper.i32 = i32;
    for (int i=0; i<4; ++i) {
        file << helper.i8s[i];
    }
    return file;
}

/**
 * @brief convertDoubleToChars
 *
 * Converts a double to characters.
 *
 * @param file the file stream to put the characters in
 * @param i32  the double to convert
 * @return the file stream
 */
std::fstream& convertDoubleToChars(std::fstream& file, double dbl)
{
    cDblto32 helper;
    helper.dbl = dbl;
    for (int i=0; i<8; ++i) {
        file << helper.i8s[i];
    }
    return file;
}
