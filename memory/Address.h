#ifndef ADDRESS_H
#define ADDRESS_H

#include <QString>

/**
 * @brief The Address class
 *
 * Used to represent an address. Each employee has an address.
 * Attributes:
 *  - country : the country of the address
 *  - provinve : the province/state/territory of the address
 *  - city : the city/town/village of the address
 *  - street : the street/avenue/road of the address
 *  - houseNumber : the street number of the address, can contain letters
 *  - postalCode : the postal code of the address
 *
 * @author Daniel Homer
 */
class Address
{
public:
    Address(QString country, QString province, QString city,
            QString street, QString houseNumber, QString postalCode);

    QString getHouseNumber();
    QString getStreet();
    QString getCity();
    QString getProvince();
    QString getCountry();
    QString getPostalCode();

    void setHouseNumber(QString houseNumber);
    void setStreet(QString street);
    void setCity(QString city);
    void setProvince(QString province);
    void setCountry(QString country);
    void setPostalCode(QString postalCode);

    QString toString();

private:
    QString houseNumber;
    QString street;
    QString city;
    QString province;
    QString country;
    QString postalCode;
};

#endif // ADDRESS_H
