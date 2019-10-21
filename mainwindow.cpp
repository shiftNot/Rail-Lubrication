#include "mainwindow.h"
#include "camera.h"
#include "ui_mainwindow.h"
#include <QtCore/qmath.h>
#include "QMessageBox"
#include "QFileDialog"
#include <QTime>
#include <QTextStream>
#include <QImage>
#include <QPainter>
#include <QImage>  
#include <QPixmap>
#include <QTimer>
#include <windows.h>
#include <stdio.h>
#include "QWindow"
#include <QDebug>
using namespace cv;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->radioButton,0);
    btnGroup->addButton(ui->radioButton_2,1);
    ui->radioButton->setChecked(true);

    btnGroup1 = new QButtonGroup(this);
    btnGroup1->addButton(ui->radioButton_3,1);//lu 1   1:0
    btnGroup1->addButton(ui->radioButton_4,2);//ld 2   2:0
    btnGroup1->addButton(ui->radioButton_8,0);

    btnGroup2 = new QButtonGroup(this);
    btnGroup2->addButton(ui->radioButton_5,1);//ru 3   0:1
    btnGroup2->addButton(ui->radioButton_6,2);//rd 4   0:2
    btnGroup2->addButton(ui->radioButton_7,0);

	btnGroup3 = new QButtonGroup(this);
	btnGroup3->addButton(ui->radioButton_10, 0);//lu 1   1:0
	btnGroup3->addButton(ui->radioButton_9, 2);//ld 2   2:0

	effect1.setSource(QUrl::fromLocalFile("1.wav"));
	effect1.setVolume(1.0f);

	effect2.setSource(QUrl::fromLocalFile("2.wav"));
	effect2.setVolume(1.0f);

    ui->radioButton_7->setChecked(true);
    ui->radioButton_8->setChecked(true);
	ui->radioButton_10->setChecked(true);

    fRead("data.txt");
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox->addItem(info.portName());
            serial.close();
        }
    }
    Sleep(2000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionclose_triggered()
{
    this->close();
}

void MainWindow::on_pushButton_clicked()
{

    static int i=0;
	code.exec();
    if(code.flag==1&&i==0){
        code.flag=0;
        i++;
        my_serialport= new QSerialPort();
        my_serialport->setPortName(ui->comboBox->currentText());
        my_serialport->open(QIODevice::ReadWrite);
        my_serialport->setBaudRate(QSerialPort::Baud19200);
        my_serialport->setDataBits(QSerialPort::Data8);
        my_serialport->setParity(QSerialPort::NoParity);
        my_serialport->setStopBits(QSerialPort::OneStop);
        my_serialport->setFlowControl(QSerialPort::NoFlowControl);
        connect(my_serialport, SIGNAL(readyRead()), this, SLOT(UpdateData()));
        code.close();
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(SendData()));
        timer->start(200);
    }
}

void MainWindow::UpdateData()
{
    int x;
    double res;
    double x1,y1,x2,y2;
    QString str;
    QFile mfile("../123.txt");
    QByteArray temp = my_serialport->readAll();
    if(!temp.isEmpty())
       {
           requestData.append(temp);
           if(requestData.contains("\r\n"))
           {
               x=1;
           }
       }
    if(x==1){	
        QString string(requestData);		
        QStringList result = string.split(",");
        x=result.count();
        if(string!= NULL&&x>8)
        {
            if(result.at(0)=="$GPRMC"){				
                if(!mfile.open(QFile::WriteOnly|QFile::Text|QFile::Append)){
                    QMessageBox::warning(this,"Warning","could not open file for writting");
                    return;
                }
                QTextStream out(&mfile);
                res=result.at(5).toDouble();
                x=res/100;
                res=(res-x*100)/60.0+x;
                out<<QString::number(res,10,7)<<"\t";
                ui->lineEdit->setText(QString::number(res,10,7));
                res=result.at(3).toDouble();
                x=res/100;
                res=(res-x*100)/60.0+x;
                out<<QString::number(res,10,7)<<"\t";
                ui->lineEdit_2->setText(QString::number(res,10,7));
                x=result.at(1).toDouble();
                ui->lineEdit_3->setText(QString::number(x/10000+8)+":"+QString::number(x/100%100)+":"+QString::number(x%100));
                res=result.at(7).toDouble();
                if(res>0.5){
                    ui->lineEdit_4->setText(QString::number(res*0.5144*3.6,10,2)+"km/h");
                    out<<result.at(8)<<"\t";
                }else{
                    ui->lineEdit_4->setText("0 km/h");
                    out<<"x"<<"\t";
                }
                out<<QString::number(res*0.5144,10,2)<<"\n";
                mfile.flush();
                mfile.close();
            }
            if(result.at(0)=="$GPGGA"){
                if(!mfile.open(QFile::WriteOnly|QFile::Text|QFile::Append)){
                    QMessageBox::warning(this,"Warning","could not open file for writting");
                    return;
                }
                QTextStream out1(&mfile);

                x=result.at(1).toDouble();
                ui->lineEdit_3->setText(QString::number(x/10000+8)+":"+QString::number(x/100%100)+":"+QString::number(x%100));
                ui->lineEdit_4->setText(result.at(7));
                res=result.at(4).toDouble();
                x=res/100;
                res=(res-x*100)/60.0+x;
                x1=res;
                if(alti>10)
                    x2=alti;
                else
                    x2=res;
                alti=res;
                out1<<QString::number(res,10,7)<<"\t";
                ui->lineEdit->setText(QString::number(res,10,7)+"E");
                res=result.at(2).toDouble();
                x=res/100;
                res=(res-x*100)/60.0+x;
                y1=res;
                if(teti>10)
                    y2=teti;
                else
                    y2=res;
                teti=res;
                out1<<QString::number(res,10,7)<<"\t";
                ui->lineEdit_2->setText(QString::number(res,10,7)+"N");
                res=sqrt(qPow((x1-x2)*100000,2)+qPow((y1-y2)*111111,2));
                if(res>0.03)
                route+=res;
                out1<<QString::number(route,10,2)<<"\n";
                mfile.flush();
                mfile.close();
            }
        }
        x=0;
		requestData.clear();
    }
   
}

void MainWindow::SendData()
{
    int flagl=0,flagr=0,flag;
    double x,y,z,w;
    QString str;
    QByteArray TxData;
    flag = btnGroup->checkedId();
    if(flag==0){
		if(btnGroup1->checkedId())
			flagl = btnGroup1->checkedId()+btnGroup3->checkedId();
		if (btnGroup2->checkedId())
			flagr = btnGroup2->checkedId()+btnGroup3->checkedId();
        str=QString().sprintf("Set:%d:%d\r\n",flagl,flagr);
        TxData=str.toLatin1();
        my_serialport->write(TxData);
    }else{//自动涂覆
		x = ui->lineEdit->text().toDouble();
		y = ui->lineEdit_2->text().toDouble();
        if(statel==0){
            flagl=0;
            for(int i=linel+1;i<rowl;i++){
                z=(vectorl[i*4]-x)*(vectorl[i*4]-x)+(vectorl[i*4+1]-y)*(vectorl[i*4+1]-y);
                if(z<0.00001&&z>0){
                    statel=1;
                    linel=i;
					delayl = 10;
                    break;
                }
            }
        }else if (statel == 1) {
			flagl = 2;
			delayl--;
			if (delayl <= 0) {
				statel = 2;
			}
		}else if(statel==2){
            flagl = 4;
            w=(vectorl[linel*4+2]-x)*(vectorl[linel*4+2]-x)+(vectorl[linel*4+3]-y)*(vectorl[linel*4+3]-y);
            z=(vectorl[linel*4]-x)*(vectorl[linel*4]-x)+(vectorl[linel*4+1]-y)*(vectorl[linel*4+1]-y);
            if(w<0.000005){
				effect1.play();
                statel=3;
                delayl=30;
            }
        }else if(statel==3){
            flagl=4;
            delayl--;
            if(delayl<=0){
                statel=4;
                delayl=5;
            }
        }else if(statel==4){
				
            flagl=0;
            delayl--;
            if(delayl<=0){
				effect2.play();
                statel=5;
                delayl=30;
            }
        }else if(statel==5){
            flagl=1;
            delayl--;
            if(delayl<=0){
                statel=0;
            }
        }
        if(stater==0){
            flagr=0;
            for(int i=liner+1;i<rowr;i++){
                z=(vectorr[i*4]-x)*(vectorr[i*4]-x)+(vectorr[i*4+1]-y)*(vectorr[i*4+1]-y);
                if(z<0.00001&&z>0){
                    stater=1;
                    liner=i;
					delayr = 10;
                    break;
                }
            }
        }
		else if (stater == 1) {
			flagr = 2;
			delayr--;
			if (delayr <= 0) {
				stater = 2;				
			}
		}
		else if(stater==2){
            flagr=4;
            w=(vectorr[liner*4+2]-x)*(vectorr[liner*4+2]-x)+(vectorr[liner*4+3]-y)*(vectorr[liner*4+3]-y);
            if(w<0.000005){
                stater=3;
                delayr=30;
            }
        }else if(stater==3){
            flagr=4;
            delayr--;
            if(delayr<=0){
                stater=4;
                delayr=5;
            }
        }else if(stater==4){
            flagr=0;
            delayr--;
            if(delayr<=0){
                stater=5;
                delayr=30;
            }
        }else if(stater==5){
            flagr=1;
            delayr--;
            if(delayr<=0){
                stater=0;
            }
        }
        if(templ+flagl==3)
            flagl=0;
        if(tempr+flagr==3)
            flagr=0;

        str=QString().sprintf("Set:%d:%d\r\n",flagl,flagr);
        TxData=str.toLatin1();
        my_serialport->write(TxData);
        templ=flagl;
        tempr=flagr;
        ui->lineEdit_5->setText("L/R:"+QString::number(statel)+"/"+QString::number(stater)+" line:"+QString::number(linel+liner+2)+"/"+QString::number(rowIndex));
    }
}


//搜索设备

void MainWindow::on_buttonSearch_clicked()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox->addItem(info.portName());//左边显示COM口
            serial.close();
        }
    }
}


void MainWindow::fRead(QString Filename)
{
    int i;
    QString linex;
    QFile mfile(Filename);
    if(!mfile.open(QFile::ReadOnly|QFile::Text)){
        QMessageBox::warning(this,"Warning","could not open file");
        return;
    }
    QTextStream in(&mfile);
    linex=in.readLine();
    rowIndex=linex.toInt();
    for(i=0;i<rowIndex;i++){
        linex=in.readLine();
        QStringList Str_List=linex.split(' ');

        if(Str_List.at(4).toInt()==1){
            for(int j=0;j<4;j++)
                vectorl[rowl*4+j]=Str_List.at(j).toDouble();
            rowl++;
        }else if(Str_List.at(4).toInt()==2){
            for(int j=0;j<4;j++)
                vectorr[rowr*4+j]=Str_List.at(j).toDouble();
            rowr++;
        }

   }
    mfile.flush();
    mfile.close();
}

//打开摄像头
void MainWindow::on_pushButton_8_clicked()
{

	ui->openGLWidget->capx.open(0);
	ui->openGLWidget_2->capx.open(1);
	QTime current_time = QTime::currentTime();
	QString StrCurrentTime = "LeftCamera" + current_time.toString("hhmmss") + ".avi";
	const char* Ldata = StrCurrentTime.toLocal8Bit();
	StrCurrentTime = "RightCamera"+current_time.toString("hhmmss")+".avi";
	const char* Rdata = StrCurrentTime.toLocal8Bit();
	if (ui->openGLWidget->capx.isOpened()) {
		ui->openGLWidget->flag = 1;
		ui->openGLWidget->vw.open(Ldata, VideoWriter::fourcc('X', '2', '6', '4'), 5, Size(ui->openGLWidget->capx.get(CAP_PROP_FRAME_WIDTH), ui->openGLWidget->capx.get(CAP_PROP_FRAME_HEIGHT)));
		if (ui->openGLWidget->vw.isOpened())
			QMessageBox::information(this, "  ", "canmera1 open successed");
	}else
		QMessageBox::information(this, "  ", "canmera1 open failed");

	if (ui->openGLWidget_2->capx.isOpened()) {
		ui->openGLWidget_2->flag = 1;
		ui->openGLWidget_2->vw.open(Rdata, VideoWriter::fourcc('X', '2', '6', '4'), 5, Size(ui->openGLWidget_2->capx.get(CAP_PROP_FRAME_WIDTH), ui->openGLWidget_2->capx.get(CAP_PROP_FRAME_HEIGHT)));
		if (ui->openGLWidget_2->vw.isOpened())
		QMessageBox::information(this, "  ", "canmera2 open successed");
	}
	else
		QMessageBox::information(this, "  ", "canmera2 open failed");
	timer1 = new QTimer(this);
	connect(timer1, SIGNAL(timeout()), this, SLOT(CameraUpData()));
	timer1->start(200);
}


void MainWindow::CameraUpData()
{
	ui->openGLWidget->update();
	ui->openGLWidget_2->update();
}
	
//关闭摄像头
void MainWindow::on_pushButton_11_clicked()
{
 //  process->close();
	timer1->stop();
}

