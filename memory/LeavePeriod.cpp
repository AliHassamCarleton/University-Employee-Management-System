#include "LeavePeriod.h"

LeavePeriod::LeavePeriod(quint32 leaveID, Date* startDate, Date* endDate, double percent)
{
    this->leaveID = leaveID;
    this->startDate = startDate;
    this->endDate = endDate;
    this->percent = percent;
}

LeavePeriod::~LeavePeriod()
{
    delete startDate;
    delete endDate;
}

quint32 LeavePeriod::getLeaveID()
{
    return (leaveID);
}

Date& LeavePeriod::getStartDate()
{
    return (*startDate);
}

Date& LeavePeriod::getEndDate()
{
    return (*endDate);
}

double LeavePeriod::getPercent()
{
    return (percent);
}

void LeavePeriod::setLeaveID(quint32 leaveID)
{
    this->leaveID = leaveID;
}

bool LeavePeriod::operator<(LeavePeriod& other)
{
    return (leaveID < other.leaveID);
}
