#include "TransactionIntegrator.h"

#include "TransactionFileHandler.h"
#include "Transaction.h"
#include "AddEmployeeTransaction.h"
#include "AddLeaveTransaction.h"
#include "AddRoleTransaction.h"
#include "ApplyRaiseTransaction.h"
#include "UpdateBasicTransaction.h"
#include "UpdatePayTransaction.h"
#include "AttributePair.h"
#include "memory/Date.h"

#include <QStringBuilder>
#include <QString>
#include <QList>

/**
 * @brief TransactionIntegrator::TransactionIntegrator
 * @param em the reference to entity manager to update the memory of the system
 */
TransactionIntegrator::TransactionIntegrator(EntityManager* em)
{
    this->entityManager = em;
    this->transactionInfo = "Transaction Info:";
}

IntegrationReturn TransactionIntegrator::initiateIntegration()
{
    TransactionFileHandler file;

    IntegrationReturn ret = UNABLE_TO_OPEN;

    if (file.isOpen()) {
        bool end = false;
        quint32 tid;


        QList<Transaction*> transactions;

        while (!end) {
            switch (tid = file.readByte()) {
                case END_OF_FILE:
                    ret = SUCCESS;
                    end = true;
                break;

                case ADD_EMPLOYEE:
                {
                    quint32 eid = file.readInt();
                    QString uid = file.readString();
                    QString givenName = file.readString();
                    QString familyName = file.readString();
                    QString houseNumber = file.readString();
                    QString street = file.readString();
                    QString postal = file.readString();
                    QString city = file.readString();
                    QString province = file.readString();
                    QString country = file.readString();

                    Address* address = new Address(country, province, city, street, houseNumber, postal);
                    transactions.append(new AddEmployeeTransaction(eid, uid, givenName, familyName, address));
                }
                break;

                case UPDATE_EMPLOYEE_BASIC:
                {
                    quint32 eid = file.readInt();
                    QList<AttributePair*>* attributes = new QList<AttributePair*>();

                    quint8 aid;

                    while ((aid = file.readByte()) != END_OF_ATTRIBUTES) {
                        QString attributeValue = file.readString();
                        attributes->append(new AttributePair((AttributeID)aid, attributeValue));
                    }

                    transactions.append(new UpdateBasicTransaction(eid, attributes));
                }

                break;

                case UPDATE_EMPLOYEE_PAY:
                {
                    quint32 eid = file.readInt();
                    quint8 roleIndex = file.readByte();
                    double amount = file.readDouble();

                    transactions.append(new UpdatePayTransaction(eid, roleIndex, amount));
                }
                break;

                case APPLY_RAISE:
                {
                    Date* applyDate = new Date(file.readInt());
                    quint8 roleType = file.readByte();
                    bool percent = (bool) file.readByte();
                    double amount = file.readDouble();

                    transactions.append(new ApplyRaiseTransaction(applyDate, roleType, percent, amount));
                }

                break;

                case ADD_LEAVE_PERIOD:
                {
                    quint32 eid = file.readInt();
                    quint8 role = file.readByte();
                    Date* start = new Date(file.readInt());
                    Date* end = new Date(file.readInt());
                    double percent = file.readDouble();

                    transactions.append(new AddLeaveTransaction(eid, role, start, end, percent));
                }
                break;

                case ADD_ROLE:
                {
                    quint32 eid = file.readInt();
                    QString title = file.readString();
                    double amount = file.readDouble();
                    Date* start = new Date(file.readInt());
                    Date* end = new Date(file.readInt());
                    quint8 type = file.readByte();

                    transactions.append(new AddRoleTransaction(eid, title, amount, start, end, type));

                }
                break;

                case EOF:
                    end = true;
                    ret = INVALID_FILE;
                break;

                default:
                    end = true;
                    ret = UNDEFINED_ISSUE;
            }
        }
        QListIterator<Transaction*> it(transactions);
        while (it.hasNext()) {
            Transaction* next = it.next();
            transactionInfo = transactionInfo % QString::fromStdString("\n") % next->displayText();
            next->apply(*entityManager);
        }

        qDeleteAll(transactions);
    }

    return (ret);
}

/**
 * @brief TransactionIntegrator::getTransactionInfo
 * @return the transaction information in a human readable way.
 */
QString TransactionIntegrator::getTransactionInfo()
{
    return (transactionInfo);
}
