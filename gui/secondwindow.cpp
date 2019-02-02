#include "secondwindow.h"
#include "ui_secondwindow.h"
#include "ApplicationLogic.h"

SecondWindow::SecondWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondWindow)
{
    ui->setupUi(this);
    appLogic= new ApplicationLogic();

    //appLogic->initiateIntegration();
}

SecondWindow::~SecondWindow()
{
    delete ui;
}



void SecondWindow::on_pushButton_clicked()
{

    //hide();
    thirdWindow= new ThirdWindow(this, appLogic);
    thirdWindow->show();


}

void SecondWindow::on_pushButton_2_clicked()
{
    fourthWindow= new fourthwindow(this);
    fourthWindow->show();


}

void SecondWindow::on_pushButton_3_clicked()
{
    dataIntegration= new DataIntegration(this, appLogic);
    dataIntegration->show();


}
