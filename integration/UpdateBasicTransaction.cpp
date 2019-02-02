#include "UpdateBasicTransaction.h"

#include <QListIterator>
#include <QStringBuilder>

UpdateBasicTransaction::UpdateBasicTransaction(quint32 eid, QList<AttributePair*>* attributes)
{
    this->eid = eid;
    this->attributes = attributes;
}

/**
 * @brief UpdateBasicTransaction::apply
 * @param em the entity management ref
 * @return true or false for success or fail
 */
bool UpdateBasicTransaction::apply(EntityManager &em)
{
    em.updateEmployeeAttributes(eid, attributes);

    qDeleteAll(*attributes);
    return (true);
}

/**
 * @brief UpdateBasicTransaction::displayText
 * @return human readable verion of the transaction
 */
QString UpdateBasicTransaction::displayText()
{
    QString ret = "Update basic - employee id: " % QString::number(eid)
              % ", number of attributes to update: " % QString::number(attributes->size());

    QListIterator<AttributePair*> it(*attributes);
    while (it.hasNext()) {
        AttributePair* ap = it.next();

        QString at;
        switch (ap->getAttributeID()) {
            case GIVEN_NAME:
                at = "Given Name";
                break;

            case FAMILY_NAME:
                at = "Family Name";
                break;

            case STREET_NUMBER:
                at = "House Number";
                break;

            case STREET_ADDRESS:
                at = "Street";
                break;

            case POSTAL_CODE:
                at = "Postal Code";
                break;

            case CITY:
                at = "City";
                break;

            case PROVINCE:
                at = "Province";
                break;

            case COUNTRY:
                at = "Country";
                break;

            case END_OF_ATTRIBUTES:
                at = "Should not happen";
        }

        ret = ret % ", " % at % " : " % ap->getValue();

    }

    return (ret);
}
