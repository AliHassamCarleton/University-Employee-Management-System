#include "AttributePair.h"

AttributePair::AttributePair(AttributeID id, QString value)
{
    this->id = id;
    this->value = value;
}

AttributeID AttributePair::getAttributeID()
{
    return (id);
}

QString AttributePair::getValue()
{
    return (value);
}

char AttributePair::idAsChar()
{
    return (char)id;
}

