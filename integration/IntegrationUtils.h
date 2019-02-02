#ifndef INTEGRATIONUTILS_H
#define INTEGRATIONUTILS_H

#include<QIntegerForSize>

/**
 * @brief IntegrationUtils header
 *
 * Enums and unions that assist in the integration process.
 *
 * @author Michael van Dyk
 */

/**
 * @brief The IntegrationReturn enum
 *
 * Represents the return given back by the the initiate integration
 * function. Gives information on if the integration was successfull
 * or what went wrong.
 */
enum IntegrationReturn {
    SUCCESS,
    NO_NEW_FILE,
    INVALID_FILE,
    UNABLE_TO_OPEN,
    UNDEFINED_ISSUE
};

/**
 * @brief The TransactionID enum
 *
 * Represents the IDs used in the transaction files to determine the
 * type of transaction.
 */
enum TransactionID {
    END_OF_FILE           = 0,
    ADD_EMPLOYEE          = 1,
    UPDATE_EMPLOYEE_BASIC = 2,
    UPDATE_EMPLOYEE_PAY   = 3,
    APPLY_RAISE           = 4,
    ADD_LEAVE_PERIOD      = 5,
    ADD_ROLE              = 6
};

/**
 * @brief The AttributeID enum
 *
 * Represents the IDs for each employee attribute.
 */
enum AttributeID {
    END_OF_ATTRIBUTES = 0,
    GIVEN_NAME        = 1,
    FAMILY_NAME       = 2,
    STREET_NUMBER     = 3,
    STREET_ADDRESS    = 4,
    POSTAL_CODE       = 5,
    CITY              = 6,
    PROVINCE          = 7,
    COUNTRY           = 8
};

/**
 * @brief The cInt8to32 union
 *
 * Used to convert between a 32bit int and 4 bytes.
 */
union cInt8to32 {
    char    i8s[4];
    quint32 i32;
};

/**
 * @brief The cDblto32 union
 *
 * Used to convert between a 64bit double and 8 bytes.
 */
union cDblto32 {
    char   i8s[8];
    double dbl;
};


#endif // INTEGRATIONUTILS_H

