#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtTask3.h"
#include<QImage>
#include<QTimer>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  
#include <opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include <iostream>

using namespace std;
using namespace cv;

class QtTask3 : public QMainWindow
{
	Q_OBJECT

public:
	QtTask3(QWidget *parent = Q_NULLPTR);
	float confThreshold; // ���Ŷ���ֵ
	float maskThreshold; //Mask threshold
	vector<string> classes;
	vector<Scalar> colors;
	// Draw the predicted bounding box ����Ԥ��ı߽��
	void drawBox(Mat& frame, int classId, float conf, Rect box, Mat& objectMask);
	//��ÿ֡��������������к���
	// Postprocess the neural network's output for each frame
	void postprocess(Mat& frame, const vector<Mat>& outs);
private:
	Ui::QtTask3Class ui;
	VideoCapture cap;
	Mat frame;
	int number;
private slots:
	void openCamera();

};
