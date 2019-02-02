#ifndef FOURTHWINDOW_H
#define FOURTHWINDOW_H

#include <QDialog>

namespace Ui {
class fourthwindow;
}

class fourthwindow : public QDialog
{
    Q_OBJECT

public:
    explicit fourthwindow(QWidget *parent = 0);
    ~fourthwindow();

private slots:
    void on_pushButton_close_clicked();

private:
    Ui::fourthwindow *ui;
};

#endif // FOURTHWINDOW_H
