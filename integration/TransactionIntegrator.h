#ifndef TRANSACTION_INTEGRATION_H
#define TRANSACTION_INTEGRATION_H

#include "IntegrationUtils.h"
#include "TransactionFileHandler.h"
#include "memory/EntityManager.h"

/**
 * @brief The TransactionIntegrator class
 *
 * Represents the transaction integration subsystem.
 * Controls the integration process.
 *
 * @author Michael van Dyk
 */
class TransactionIntegrator
{
private:
    EntityManager* entityManager;   // The link to the entity management subsystem
    QString        transactionInfo; // Human readable string of the transactions performed
public:
    TransactionIntegrator(EntityManager* em);

    IntegrationReturn initiateIntegration(); // Starts the integration process.
    QString           getTransactionInfo();  // Gives back the readable transaction information.
};

#endif
