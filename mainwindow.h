#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog_code.h"
#include <QButtonGroup>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionclose_triggered();

    void UpdateData();

	void CameraUpData();

    void on_pushButton_clicked();

    void SendData();

    void on_pushButton_8_clicked();

    void on_pushButton_11_clicked();

    void on_buttonSearch_clicked();//搜索设备

private:
    Ui::MainWindow *ui;
    Dialog_code code;
    QSerialPort *my_serialport;
    QByteArray requestData;
    QTimer *timer;
	QTimer *timer1;
	QMediaPlayer player;
	QSoundEffect effect1;
	QSoundEffect effect2;
    double vectorl[50];
    double vectorr[50];
    int templ;
    int tempr;
    int stater=0;
    int statel=0;
    int rowIndex=0;
    int rowr=0;
    int rowl=0;
    int linel=-1;
    int liner=-1;
    int delayl=0;
    int delayr=0;
    int row;
    QButtonGroup *btnGroup;
    QButtonGroup *btnGroup1;
    QButtonGroup *btnGroup2;
	QButtonGroup *btnGroup3;
    int station;
    double route=0;
    double alti;
    double teti;
    void fRead(QString Filename);
};

#endif // MAINWINDOW_H
