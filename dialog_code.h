#ifndef DIALOG_CODE_H
#define DIALOG_CODE_H

#include <QDialog>

namespace Ui {
class Dialog_code;
}

class Dialog_code : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_code(QWidget *parent = 0);
    ~Dialog_code();
    int flag;
    QString senddata;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dialog_code *ui;
};

#endif // DIALOG_CODE_H
