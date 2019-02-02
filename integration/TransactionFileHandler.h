#ifndef TRANSACTIONFILEHANDLER_H
#define TRANSACTIONFILEHANDLER_H

#include<QIntegerForSize>
#include<fstream>

/**
 * @brief The TransactionFileHandler class
 *
 * Handles the interaction with the transaction file. Has functions
 * to handle reading from the file.
 *
 * @author Michael van Dyk
 */
class TransactionFileHandler
{
public:
    TransactionFileHandler();
    ~TransactionFileHandler();

    bool isOpen();

    quint32 readByte();   // Read 8 bits from the file
    double  readDouble(); // Read a 64 bit decimal from the file
    quint32 readInt();    // Read an 32 bit int from the file
    QString readString(); // Read a null delimited string from the file.

private:
    std::fstream file; //the file connection being read from
};

#endif // TRANSACTIONFILEHANDLER_H
