#include "StorageConnection.h"

#include <QSqlQuery>

#include <fstream>
#include <iostream>

StorageConnection::StorageConnection()
{
    static bool open = false;
    if (open) {
        db = QSqlDatabase::database();
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("localhost");
        db.setDatabaseName("./cuNICS.db");
        open = db.open();
    }
}

StorageConnection::~StorageConnection()
{
    db.close();
}
