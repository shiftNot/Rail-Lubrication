#include "camera.h"
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QPainter>

using namespace cv;

camera::camera(QWidget *p):QOpenGLWidget(p)
{
}


camera::~camera()
{
}

void camera::paintEvent(QPaintEvent *e)
{
	Mat frame;
	if (capx.read(frame)) {
		vw.write(frame);
		cvtColor(frame, frame, COLOR_BGR2RGB);
		QImage img(frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
		QPainter painter;
		painter.begin(this);
		painter.drawImage(QPoint(25, -30), img);
		painter.end();
	}
}