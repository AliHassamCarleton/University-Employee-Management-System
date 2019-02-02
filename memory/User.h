#ifndef USER
#define USER

#include <QString>

/**
 * @brief The User class
 *
 * @author Daniel Homer
 */
class User
{
public:
    QString getUserID();

protected:
    User(QString id);

private:
    QString userID;
};

#endif // USER
