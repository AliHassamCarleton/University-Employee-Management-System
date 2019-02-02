#include "IntegrationUtils.h"

#include <QString>

quint32 convertCharsToInt32(char i8s[4])
{
    cInt8to32 helper;
    for (int i=0; i<4; ++i)
        helper.i8s[i] = i8s[i];
    return helper.i32;
}

//todo delete file
