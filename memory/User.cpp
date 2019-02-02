#include "User.h"

User::User(QString userID)
{
    this->userID = userID;
}

QString User::getUserID(){ return userID;}
