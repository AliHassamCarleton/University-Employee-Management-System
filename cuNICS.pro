#-------------------------------------------------
#
# Project created by QtCreator 2017-03-01T11:49:02
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cuNICS
TEMPLATE = app

SOURCES += main.cpp\
    database/DatabaseControl.cpp \
    memory/Address.cpp \
    memory/ContinuingRole.cpp \
    memory/Date.cpp \
    memory/Deduction.cpp \
    memory/Employee.cpp \
    memory/EmployeeRole.cpp \
    memory/EntityControl.cpp \
    memory/LeavePeriod.cpp \
    memory/Raise.cpp \
    memory/TermRole.cpp \
    memory/User.cpp \
    integration/Transaction.cpp \
    integration/TransactionFileCreator.cpp \
    integration/AttributePair.cpp \
    integration/TransactionFileHandler.cpp \
    integration/AddEmployeeTransaction.cpp \
    integration/UpdateBasicTransaction.cpp \
    integration/UpdatePayTransaction.cpp \
    integration/ApplyRaiseTransaction.cpp \
    integration/AddLeaveTransaction.cpp \
    memory/EntityManager.cpp \
    database/StorageManager.cpp \
    integration/TransactionIntegrator.cpp \
    database/StorageConnection.cpp \
    integration/AddRoleTransaction.cpp \
    gui/mainwindow.cpp \
    gui/secondwindow.cpp \
    gui/thirdwindow.cpp \
    gui/fourthwindow.cpp \
    gui/dataintegration.cpp \
    ApplicationLogic.cpp \
    memory/PayrollSummary.cpp



HEADERS  += database/DatabaseControl.h \
    memory/Address.h \
    memory/ContinuingRole.h \
    memory/Date.h \
    memory/Deduction.h \
    memory/Employee.h \
    memory/EmployeeRole.h \
    memory/EntityControl.h \
    memory/LeavePeriod.h \
    memory/Raise.h \
    memory/TermRole.h \
    memory/User.h \
    integration/Transaction.h \
    integration/TransactionFileCreator.h \
    integration/AttributePair.h \
    integration/IntegrationUtils.h \
    integration/TransactionFileHandler.h \
    integration/AddEmployeeTransaction.h \
    integration/UpdateBasicTransaction.h \
    integration/UpdatePayTransaction.h \
    integration/ApplyRaiseTransaction.h \
    integration/AddLeaveTransaction.h \
    memory/EntityManager.h \
    database/StorageManager.h \
    integration/TransactionIntegrator.h \
    database/StorageConnection.h \
    integration/AddRoleTransaction.h \
    gui/mainwindow.h \
    gui/secondwindow.h \
    gui/thirdwindow.h \
    gui/fourthwindow.h \
    gui/dataintegration.h \
    ApplicationLogic.h \
    memory/PayrollSummary.h

FORMS    += gui/mainwindow.ui \
    gui/secondwindow.ui \
    gui/thirdwindow.ui \
    gui/fourthwindow.ui \
    gui/dataintegration.ui


DISTFILES += cuNICSdb.sql \
    cuNICS.db
