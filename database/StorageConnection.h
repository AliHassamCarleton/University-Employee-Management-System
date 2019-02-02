#ifndef STORAGECONNECTION_H
#define STORAGECONNECTION_H

#include <QSqlDatabase>
#include <QIntegerForSize>

class StorageConnection
{
public:
    StorageConnection();
    ~StorageConnection();

private:
    QSqlDatabase db;

};

#endif // STORAGECONNECTION_H
