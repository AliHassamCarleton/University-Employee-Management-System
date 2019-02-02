#include "Address.h"

/**
 * @brief Address::Address
 *      Creates a new address object initialized with the given parameters.
 *
 * @param country : the country of the address
 * @param province : the province of the address
 * @param city : the city of the address
 * @param street : the street of the address
 * @param houseNumber : the street number of the address
 * @param postalCode : the postal code of the address
 */
Address::Address(QString country, QString province, QString city, QString street, QString houseNumber, QString postalCode)
{
    this->country = country;
    this->province = province;
    this->city = city;
    this->street = street;
    this->houseNumber = houseNumber;
    this->postalCode = postalCode;
}

/**
 * @brief Address::toString
 *      Returns the address in a single string.
 *
 * @return the addressin a single string
 */
QString Address::toString()
{
    return houseNumber + " " + street + ", " + city + " " + province + " " + country + ", " + postalCode;
}

/**
 * @brief Address::getCity
 * @return the city of the address
 */
QString Address::getCity()           {return city;}

/**
 * @brief Address::getCountry
 * @return the country of the address
 */
QString Address::getCountry()        {return country;}

/**
 * @brief Address::getHouseNumber
 * @return the house number of the address
 */
QString Address::getHouseNumber()    {return houseNumber;}

/**
 * @brief Address::getProvince
 * @return the province of the address
 */
QString Address::getProvince()       {return province;}

/**
 * @brief Address::getStreet
 * @return the street of the address
 */
QString Address::getStreet()         {return street;}

/**
 * @brief Address::getPostalCode
 * @return the postal code of the address
 */
QString Address::getPostalCode()     {return postalCode;}

void Address::setHouseNumber(QString houseNumber)
{
    this->houseNumber = houseNumber;
}

void Address::setStreet(QString street)
{
    this->street = street;
}

void Address::setCity(QString city)
{
    this->city = city;
}

void Address::setProvince(QString province)
{
    this->province = province;
}

void Address::setCountry(QString country)
{
    this->country = country;
}

void Address::setPostalCode(QString postalCode)
{
    this->postalCode = postalCode;
}
