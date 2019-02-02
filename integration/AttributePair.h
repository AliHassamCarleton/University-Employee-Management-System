#ifndef ATTRIBUTEPAIR_H
#define ATTRIBUTEPAIR_H

#include <QString>

#include "IntegrationUtils.h"

/**
 * @brief The AttributePair class
 *
 * Makes a pair of an AttributeID and a string value that represents an update that
 * is to be made to a specific attribute of an employee.
 *
 * @author Michael van Dyk
 */
class AttributePair {
public:
    AttributePair(AttributeID id, QString value);

    AttributeID getAttributeID();
    QString     getValue();

    char idAsChar();   // Get the ID as a character for putting in a file

private:
    AttributeID id;    // the AttributeID
    QString     value; // The value to change to

};

#endif // ATTRIBUTEPAIR_H
