#ifndef APPLICATIONLOGIC_H
#define APPLICATIONLOGIC_H

#include <QString>
#include <QList>

#include "memory/EntityManager.h"
#include "database/StorageManager.h"

#include "integration/TransactionFileCreator.h"
#include "integration/TransactionIntegrator.h"

/**
 * @brief The Application class
 *
 * @author Daniel Homer
 */
class ApplicationLogic
{
private:
    TransactionIntegrator* transaction;
public:
    ApplicationLogic();
    EntityManager entityManager;
    /*
     * addDeduction
     * addEmployee
     * addEmployeeRole
     * addLeavePeriod
     * applyFixedRaise
     * applyPercentRaise
     * getEmployeeList
     * getEmployeeListFiltered
     * getPayrollSummary
     * initiateIntegration
     * updateEmployee
     * updateEmployeeRole
     */
    void initiateIntegration();
    void updateEmployeeAttributes(quint32 eid, QList<AttributePair*>* attributes);
    QString getTransactionInfo();
    QList<Employee*> getEmployeeList();
};

#endif // APPLICATIONLOGIC_H
