#ifndef LEAVEPERIOD_H
#define LEAVEPERIOD_H

#include "Date.h"

/**
 * @brief The LeavePeriod class
 *
 * @author Daniel Homer
 * @author Michael van Dyk
 */
class LeavePeriod
{
private:
    quint32 leaveID;
    Date* startDate;
    Date* endDate;
    double percent;

public:
    LeavePeriod(quint32 leaveID, Date* startDate, Date* endDate, double percent);
    ~LeavePeriod();

    quint32 getLeaveID();
    Date& getStartDate();
    Date& getEndDate();
    double getPercent();

    void setLeaveID(quint32 leaveID);

    bool operator<(LeavePeriod& other);
};

#endif // LEAVEPERIOD_H
