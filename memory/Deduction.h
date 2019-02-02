#ifndef DEDUCTION_H
#define DEDUCTION_H

#include <QString>
#include <QIntegerForSize>

/** Deduction boolean values for the percent attribute */
#define DEDUCTION_FIXED   false
#define DEDUCTION_PERCENT true

/**
 * @brief The Deduction class
 * Stores the information related to a deduction.
 * Attributes:
 *  - deductionID : reference to the deduction in the EntityControl's Deductions list
 *  - deductionName : a distinguishing name for the deduction
 *  - percent : used to determine if the deduction is fixed amount or percentage based
 *  - deductionAmount : the amount for the deduction, percent if percentage based, dollar amount otherwise
 *
 * @author Michael van Dyk
 */
class Deduction
{
public:
    Deduction(quint32 deductionID, QString deductionName, bool percent, double deductionAmount);

    quint32 getDeductionID();
    QString getDeductionName();
    double  getDeductionAmount();

    bool isFixedAmount();
    bool isPercentage();

private:
    quint32 deductionID;
    QString deductionName;
    bool    percent;
    double  deductionAmount;

};

#endif // DEDUCTION_H
