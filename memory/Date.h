#ifndef DATE_H
#define DATE_H

#include<QIntegerForSize>

/**
 * @brief The Date class
 * A simple date class.
 *
 * Attributes:
 *  - day : the day the date represents
 *  - month : the month the date represents
 *  - year : the year the date represents
 *
 * @author Daniel Homer
 * @author Michael van Dyk
 */
class Date
{
public:
    Date();
    Date(quint32 day, quint32 month, quint32 year);
    Date(quint32 dval);

    quint32 getDay();
    quint32 getMonth();
    quint32 getYear();

    quint32 toInteger();

    QString toString();

    /**
     * @brief monthStrings
     * Holds the names of the months.
     */
    static const QString monthStrings[];
private:
    quint32 day;
    quint32 month;
    quint32 year;
};

#endif // DATE_H
