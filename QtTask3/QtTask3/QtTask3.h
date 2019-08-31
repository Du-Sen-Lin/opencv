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
	float confThreshold; // 置信度阈值
	float maskThreshold; //Mask threshold
	vector<string> classes;
	vector<Scalar> colors;
	// Draw the predicted bounding box 绘制预测的边界框
	void drawBox(Mat& frame, int classId, float conf, Rect box, Mat& objectMask);
	//对每帧的神经网络输出进行后处理
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
