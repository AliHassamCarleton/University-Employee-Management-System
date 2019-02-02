#ifndef DATAINTEGRATION_H
#define DATAINTEGRATION_H
#include <iostream>
#include "ApplicationLogic.h"

#include <QDialog>

namespace Ui {
class DataIntegration;
}

class DataIntegration : public QDialog
{
    Q_OBJECT


public:
    explicit DataIntegration(QWidget *parent = 0, ApplicationLogic* appLogic=NULL);

    ~DataIntegration();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DataIntegration *ui;
    ApplicationLogic* appLogic;

};

#endif // DATAINTEGRATION_H
