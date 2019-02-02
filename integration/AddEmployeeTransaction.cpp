#include "AddEmployeeTransaction.h"

#include "memory/Address.h"

#include <QStringBuilder>

/**
 * @brief AddEmployeeTransaction::AddEmployeeTransaction
 *
 * Creates a new employee transaction object.
 *
 * @param eid        the employee id of the new employee
 * @param uid        the username of the new employee
 * @param givenName  the given name of the new employee
 * @param familyName the family name of the new employee
 * @param address    the address of the new employee
 */
AddEmployeeTransaction::AddEmployeeTransaction(quint32 eid, QString uid, QString givenName, QString familyName, Address* address)
{
    this->eid = eid;
    this->uid = uid;
    this->givenName = givenName;
    this->familyName = familyName;
    this->address = address;
}

/**
 * @brief AddEmployeeTransaction::apply
 * @param em the entity management ref
 * @return true or false for success or fail
 */
bool AddEmployeeTransaction::apply(EntityManager &em)
{
    // Create the employee and give the new employee to the entity management to update cuNICS
    Employee* tmpE = new Employee(uid,eid,givenName,familyName,address);
    em.addEmployee(tmpE);

    return (true);
}

/**
 * @brief AddEmployeeTransaction::displayText
 * @return human readable verion of the transaction
 */
QString AddEmployeeTransaction::displayText()
{
    return ("Add employee - id: " % QString::number(eid) % ", username: " % uid % ", given name: " % givenName % ", family name: " % familyName
            % ", house num: " % address->getHouseNumber() %  ", street: " % address->getStreet() % ", city: " % address->getCity()
            % ", postal: " % address->getPostalCode() % ", province: " % address->getProvince() % ", country: " % address->getCountry());
}
