#include "Deduction.h"

/**
 * @brief Deduction::Deduction
 *      Initializes the deduction object based on the given parameters.
 * @param deductionID : the db id of the deduction
 * @param deductionName : the name of the deduction
 * @param percent : if the deduction is precent or fixed amount based
 * @param deductionAmount : the amount of the deduction
 */
Deduction::Deduction(quint32 deductionID, QString deductionName, bool percent, double deductionAmount)
{
    this->deductionID = deductionID;
    this->deductionName = deductionName;
    this->percent = percent;
    this->deductionAmount = deductionAmount;
}

/**
 * @brief Deduction::getDeductionID
 * @return the deduction id
 */
quint32 Deduction::getDeductionID()
{
    return(deductionID);
}

/**
 * @brief Deduction::getDeductionName
 * @return the name of the deduction
 */
QString Deduction::getDeductionName()
{
    return (deductionName);
}

/**
 * @brief Deduction::getDeductionAmount
 * @return the deduction amount
 */
double  Deduction::getDeductionAmount()
{
    return(deductionAmount);
}

/**
 * @brief Deduction::isFixedAmount
 * @return if the deduction is a fixed amount
 */
bool Deduction::isFixedAmount()
{
    return (percent == DEDUCTION_FIXED);
}

/**
 * @brief Deduction::isPercentage
 * @return if the deduction is percentage based
 */
bool Deduction::isPercentage()
{
    return (percent == DEDUCTION_PERCENT);
}
