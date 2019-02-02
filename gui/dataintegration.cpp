#include "dataintegration.h"
#include <QMessageBox>
#include "ui_dataintegration.h"
#include "ApplicationLogic.h"
#include <QTextBrowser>
#include <iostream>

DataIntegration::DataIntegration(QWidget *parent, ApplicationLogic* appLogicin) :
    QDialog(parent),
    ui(new Ui::DataIntegration)
{

    appLogic= appLogicin;
    ui->setupUi(this);


}

DataIntegration::~DataIntegration()
{
    delete ui;
}

void DataIntegration::on_pushButton_clicked()
{

    //decide on sucess or fail
    QMessageBox::information(this,"Data Integration","Data Integration complete");

    //else fail


    //update the list widget with information
    //ApplicationLogic appLogic;
    appLogic->initiateIntegration();

    ui->textBrowser->append(appLogic->getTransactionInfo());





}

void DataIntegration::on_pushButton_2_clicked()
{

    hide();

}
