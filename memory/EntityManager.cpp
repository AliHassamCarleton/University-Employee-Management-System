#include "EntityManager.h"

EntityManager::EntityManager()
{
    employees = new QMap<quint32, Employee*>();
    raises = new QList<Raise*>();
    storageManager.getEmployees(employees);
}

EntityManager::~EntityManager(){
    qDeleteAll(*employees);
    qDeleteAll(*raises);

    delete employees;
    delete raises;
}

//PUBLIC INTERFACE
bool EntityManager::addDeduction(quint32 rtype, Deduction* d){

    //add the deduction to the specified EmployeeRole in persistent storage
    storageManager.addDeduction(rtype,*d);
    return 0;
}

bool EntityManager::addEmployee(Employee* e){
    if (e->getEmployeeID() == 0) {
        e->setEmployeeID(storageManager.getNextEmployeeNumber());
    }
    //add the employee to the list in memory

    employees->insert(e->getEmployeeID(),e);


    //add the employee to the persistent storage
    storageManager.addEmployee(*e);
    return 0;
}

bool EntityManager::addEmployeeRole(quint32 eid, EmployeeRole* er){
   //add the EmployeeRole to the Employee in memory
    employees->value(eid)->addEmployeeRole(er);
    er->setRoleID(storageManager.getNextRoleNumber());

   //add the EmployeeRole to the Employee in persistent storage
    storageManager.addEmployeeRole(eid,*er);
    return 0;
}

bool EntityManager::addLeavePeriod(ContinuingRole* cr, LeavePeriod* lp){
    //add the LeavePeriod to a ContinuingRole object (belonging to an Employee) in memory
    lp->setLeaveID(storageManager.getNextLeaveNumber());
    cr->addLeavePeriod(lp);
    //add the LeavePeriod to a ContinuingRole object (belonging to an Employee) in persistent storage
    storageManager.addLeavePeriod(*cr, *lp);
    return 0;
}

bool EntityManager::applyRaise(Raise* r){
    //apply the raise in memory
    raises->append(r);

    //apply the raise in persistent storage
    storageManager.applyRaise(*r);
    return 0;
}


Employee* EntityManager::getEmployee(quint32 eid){
    return employees->value(eid);
}

QList<Employee*> EntityManager::getEmployeeList(){
    return employees->values();
}

QList<Employee*> EntityManager::getEmployeeListFiltered(quint32 attNum){
    //temp
    return employees->values();
}

bool EntityManager::updateEmployee(Employee* e){
    storageManager.updateEmployee(*e);
    return 0;
}

bool EntityManager::updateEmployeeAttributes(quint32 eid, QList<AttributePair*>* attributes)
{
    Employee* emp = employees->value(eid);

    QListIterator<AttributePair*> it(*attributes);
    while (it.hasNext()) {
        AttributePair *ap = it.next();

        switch (ap->getAttributeID()) {
            case GIVEN_NAME:
                emp->setGivenName(ap->getValue());
                break;
            case FAMILY_NAME:
                emp->setFamilyName(ap->getValue());
                break;
            case STREET_NUMBER:
                emp->getAddress().setHouseNumber(ap->getValue());
                break;
            case STREET_ADDRESS:
                emp->getAddress().setStreet(ap->getValue());
                break;
            case POSTAL_CODE:
                emp->getAddress().setPostalCode(ap->getValue());
                break;
            case CITY:
                emp->getAddress().setCity(ap->getValue());
                break;
            case PROVINCE:
                emp->getAddress().setProvince(ap->getValue());
                break;
            case COUNTRY:
                emp->getAddress().setCountry(ap->getValue());
                break;
            default:
                break;
        }
    }

    storageManager.updateEmployeeAttributes(eid, attributes);
    return 0;
}

bool EntityManager::updateEmployeeRole(EmployeeRole* er){
    storageManager.updateEmployeeRole(*er);
    return 0;
}

