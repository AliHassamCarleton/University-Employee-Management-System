#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>
#include "thirdwindow.h"
#include "fourthwindow.h"
#include "dataintegration.h"
#include "ApplicationLogic.h"

namespace Ui {
class SecondWindow;
}

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent =0);
    ~SecondWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::SecondWindow *ui;
    ThirdWindow *thirdWindow;
    fourthwindow *fourthWindow;
    DataIntegration *dataIntegration;
    ApplicationLogic* appLogic;
};

#endif // SECONDWINDOW_H
