#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtTask2.h"

#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/opencv.hpp>
#include "opencv2/highgui.hpp"
#include <iostream>
#include <qlabel.h> 
#include <qpushbutton.h>
#include <fstream>
#include <sstream>
#include <QFile>
#include <qdebug.h>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include<QImage>
#include<QTimer>
using namespace std;
using namespace cv;
using namespace dnn;
class QtTask2 : public QMainWindow
{
	Q_OBJECT

public:
	QtTask2(QWidget *parent = Q_NULLPTR);
	float confThreshold;
	float nmsThreshold;
	std::vector<std::string> classes;
	void postprocess(Mat& frame, const std::vector<Mat>& out, Net& net);
	void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame);
private:
	Ui::QtTask2Class ui;
	VideoCapture cap;
	Mat frame;
	int number;
private slots:
	void OnClicked();
};
