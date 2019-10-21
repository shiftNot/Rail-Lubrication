#pragma once
#include<QOpenGLWidget>
#include <opencv2/opencv.hpp>
class camera:public QOpenGLWidget
{
	Q_OBJECT
public:
	camera(QWidget *p);
	~camera();
	void paintEvent(QPaintEvent *e);
	int flag;
	cv::VideoCapture capx;
	cv::VideoWriter vw;
};

