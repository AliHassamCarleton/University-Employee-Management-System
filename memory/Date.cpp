#include "Date.h"

#include <QString>

/**
 * Implementaion note: a date value is a compress form of a date that stores the
 * day in the lowest 5 bits, month in the next 4 lowest bits and the year in the
 * remaining bits.
 */

const QString Date::monthStrings[] = { "January", "February", "March", "April",
                    "May", "June", "July", "August", "September",
                    "October", "November", "December" };
/**
 * @brief Date::Date
 * TODO remove?
 */
Date::Date()
{
    day   = 1;
    month = 1;
    year  = 1;
}

/**
 * @brief Date::Date
 *      Initializes a date with the given values.
 * @param day : the day that the date will represent
 * @param month : the month that the date will represent
 * @param year : the year that the date will represent
 */
Date::Date(quint32 day, quint32 month, quint32 year)
{
    this->day = day;
    this->month = month;
    this->year = year;
}

/**
 * @brief Date::Date
 *      Intializes a date with the given date value.
 * @param dval : the date value
 */
Date::Date(quint32 dval)
{
    year = dval >> 9;
    month = dval >> 5 & 0xF;
    day = dval & 0x1F;
}

/**
 * @brief Date::getDay
 * @return the day of the date
 */
quint32 Date::getDay()  {return day;}

/**
 * @brief Date::getMonth
 * @return the month of the date
 */
quint32 Date::getMonth(){return month;}

/**
 * @brief Date::getYear
 * @return the year of the date
 */
quint32 Date::getYear() {return year;}

/**
 * @brief Date::toInteger
 * @return the date as an integer date value
 */
quint32 Date::toInteger()
{
    return ((year << 9) | (month << 5) | day);
}

/**
 * @brief Date::toString
 * @return the date in a string form
 */
QString Date::toString()
{
    return (monthStrings[month-1] + " " + QString::number(day) + ", " +  QString::number(year));
}
