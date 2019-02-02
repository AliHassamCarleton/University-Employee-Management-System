#include "thirdwindow.h"
#include"secondwindow.h"
#include "ui_thirdwindow.h"
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDateEdit>
#include<QTextBrowser>
#include <iostream>
#include <QString>
#include <ApplicationLogic.h>

ThirdWindow::ThirdWindow(QWidget *parent, ApplicationLogic* appLogicin) :
    QDialog(parent),
    ui(new Ui::ThirdWindow)
{
    ui->setupUi(this);

    appLogic= appLogicin;
    //createEmployeeList();

}

ThirdWindow::~ThirdWindow()
{
    delete ui;
}

//compares dates to info
/*void ThirdWindow::createEmployeeList(){


    //compare date month year to data and then output into the box
    QStandardItemModel *model= new QStandardItemModel(0,0,this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Date")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Gross Income")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Deductions")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Net Income")));

    //createEmployeInfo();
    model->setVerticalHeaderItem(0, new QStandardItem(QString("bob loblaw")));
    model->setItem(0,0, new QStandardItem(QString("Date")));
    model->setItem(0,1, new QStandardItem(QString("Grosss")));
    model->setItem(0,2, new QStandardItem(QString("Ded")));
    model->setItem(0,3, new QStandardItem(QString("New")));

    ui->tableView->setModel(model);



}
*/








void ThirdWindow::on_pushButton_Back_clicked()
{

    hide();


}



void ThirdWindow::on_pushButton_clicked()
{

    int i;
    int k;
    QString emp;

   // ApplicationLogic appLogic;
   // appLogic.initiateIntegration();

    QList<Employee *> list=appLogic->getEmployeeList();


    for (i=0; i<list.size();i++){


        emp.append("Employee ID: ");
        QString temp= QString::number(list.at(i)->getEmployeeID());
        emp.append(temp);
        emp.append("  |    Name: ");
        emp.append(list.at(i)->getGivenName());
        emp.append("  |  \nRoles: ");

        QList<EmployeeRole *> listrole=list.at(i)->getEmployeeRoles();


        for(k=0;k<listrole.size();k++){

            emp.append(listrole.at(k)->getRoleName());
            emp.append(listrole.at(k)->toString());
            emp.append(",    $");
            QString temp= QString::number(listrole.at(k)->getSalary());
            emp.append(temp);
            emp.append(",  ");



        }


        emp.append(" \n\n\n\n");


    }
    ui->textBrowser->append(emp);


    //ui->textBrowser->(appLogic.getEmployeeList().);




}
