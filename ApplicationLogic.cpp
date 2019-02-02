#include "ApplicationLogic.h"
#include<iostream>

ApplicationLogic::ApplicationLogic()
{

}
void ApplicationLogic::initiateIntegration(){
//This function integrates the transaction file with memory/persistent storage
    TransactionFileCreator::makeSampleTransactionFile();

    transaction = new TransactionIntegrator(&entityManager);

    std::cout << transaction->initiateIntegration() << std::endl;
}

QString ApplicationLogic::getTransactionInfo(){
    return transaction->getTransactionInfo();
}

void ApplicationLogic::updateEmployeeAttributes(quint32 eid, QList<AttributePair*>* attributes ){

}

QList<Employee *> ApplicationLogic::getEmployeeList(){
    return entityManager.getEmployeeList();
}
