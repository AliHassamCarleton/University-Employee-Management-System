#include "fourthwindow.h"
#include "ui_fourthwindow.h"

fourthwindow::fourthwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fourthwindow)
{
    ui->setupUi(this);
}

fourthwindow::~fourthwindow()
{
    delete ui;
}

void fourthwindow::on_pushButton_close_clicked()
{
    hide();

}
