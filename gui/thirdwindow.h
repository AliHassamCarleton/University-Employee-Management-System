#ifndef THIRDWINDOW_H
#define THIRDWINDOW_H
#include <QDialog>
#include "ApplicationLogic.h"

namespace Ui {
class ThirdWindow;
}

class ThirdWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ThirdWindow(QWidget *parent = 0, ApplicationLogic* appLogic=NULL);
    ~ThirdWindow();

private slots:
    void on_pushButton_Back_clicked();

    void on_pushButton_clicked();


private:
    Ui::ThirdWindow *ui;
    ApplicationLogic* appLogic;
    quint32 m,d,y;

};

#endif // THIRDWINDOW_H
