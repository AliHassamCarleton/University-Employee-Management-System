#include "TransactionFileHandler.h"
#include "IntegrationUtils.h"

#include <sstream>
#include <QString>

/**
 * @brief TransactionFileHandler::TransactionFileHandler
 * Opens the transaction file to be read into the system.
 */
TransactionFileHandler::TransactionFileHandler()
{
    file.open("transaction.txt", std::fstream::in);
}

/**
 * @brief TransactionFileHandler::~TransactionFileHandler
 * Closes the connection to the transaction file.
 */
TransactionFileHandler::~TransactionFileHandler()
{
    file.close();
}

/**
 * @brief TransactionFileHandler::isOpen
 * @return true if the file is open.
 */
bool TransactionFileHandler::isOpen()
{
    return (file.is_open());
}

/**
 * @brief TransactionFileHandler::readByte
 *
 * Reads the next byte in the file (8 bits).
 * @return the next byte in the file
 */
quint32 TransactionFileHandler::readByte()
{
    return (file.get());
}

/**
 * @brief TransactionFileHandler::readDouble
 *
 * Reads the next double in the file (8 bytes)
 * @return the next double
 */
double  TransactionFileHandler::readDouble()
{
    cDblto32 helper;
    for (int i=0; i<8; ++i) {
        helper.i8s[i] = (char)file.get();
    }
    return (helper.dbl);
}

/**
 * @brief TransactionFileHandler::readInt
 *
 * Reads the next 32bit integer (4 bytes)
 * @return the next 4 bytes as an unsigned integer
 */
quint32 TransactionFileHandler::readInt()
{
    cInt8to32 helper;
    for (int i=0; i<4; ++i) {
        helper.i8s[i] = (char)file.get();
    }
    return (helper.i32);
}

/**
 * @brief TransactionFileHandler::readString
 *
 * Reads in the next bytes until a null character is found.
 * They are interpreted as a c-string in a QString.
 * @return the next string in the file
 */
QString TransactionFileHandler::readString()
{
    std::stringstream ss("");
    char c;

    while ((c = (char)file.get()) != 0) {
        ss << c;
    }

    return (QString::fromStdString(ss.str()));
}

