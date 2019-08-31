#include "QtTask3.h"
#include <fstream>
#include <sstream>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>

#include "common.hpp"
#ifdef CV_CXX11
#include <mutex>
#include <thread>
#include <queue>
#endif

using namespace std;
using namespace dnn;
using namespace cv;


QtTask3::QtTask3(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.btn_open, SIGNAL(clicked), this, SLOT(openCamera()));
	number = 0;
	confThreshold = 0.5;
	maskThreshold = 0.3;
}

void QtTask3::openCamera() 
{

	// Load names of classes
	string classesFile = "mscoco_labels.names";
	ifstream ifs(classesFile.c_str());
	string line;
	while (getline(ifs, line)) classes.push_back(line);

	// Load the colors
	//vector<Scalar> colors;
	string colorsFile = "colors.txt";
	ifstream colorFptr(colorsFile.c_str());
	while (getline(colorFptr, line)) {
		char* pEnd;
		double r, g, b;
		r = strtod(line.c_str(), &pEnd);
		g = strtod(pEnd, NULL);
		b = strtod(pEnd, NULL);
		Scalar color = Scalar(r, g, b, 255.0);  //white
		colors.push_back(Scalar(r, g, b, 255.0));
	}

	// Give the configuration and weight files for the model
	String textGraph = "./mask_rcnn_inception_v2_coco_2018_01_28.pbtxt";
	String modelWeights = "./mask_rcnn_inception_v2_coco_2018_01_28/frozen_inference_graph.pb";

	// Load the network
	Net net = readNetFromTensorflow(modelWeights, textGraph);
	net.setPreferableBackend(DNN_BACKEND_OPENCV);
	net.setPreferableTarget(DNN_TARGET_CPU);
	// Open a video file or an image file or a camera stream.
	string str;
	VideoCapture cap;
	VideoWriter video;
	Mat frame, blob;

	str = "cars.mp4";
	ifstream ifile(str);
	if (!ifile) throw("error");
	cap.open(str);

	// Process frames.
	while (waitKey(1) < 0)
	{
		// get frame from the video
		cap >> frame;

		// Stop the program if reached end of video
		if (frame.empty()) {
			cout << "Done processing !!!" << endl;
			//cout << "Output file is stored as " << outputFile << endl;
			waitKey(3000);
			break;
		}
		else {
			QImage image((const uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
		}
		// Create a 4D blob from a frame.
		blobFromImage(frame, blob, 1.0, Size(frame.cols, frame.rows), Scalar(), true, false);

		//Sets the input to the network
		net.setInput(blob);

		// Runs the forward pass to get output from the output layers
		std::vector<String> outNames(2);
		outNames[0] = "detection_out_final";
		outNames[1] = "detection_masks";
		vector<Mat> outs;
		net.forward(outs, outNames);

		// Extract the bounding box and mask for each of the detected objects
		postprocess(frame, outs);

		// Put efficiency information. The function getPerfProfile returns the overall time for inference(t) and the timings for each of the layers(in layersTimes)
		vector<double> layersTimes;
		double freq = getTickFrequency() / 1000;
		double t = net.getPerfProfile(layersTimes) / freq;
		string label = format("Mask-RCNN : Inference time for a frame : %.2f ms", t);
		putText(frame, label, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
		cvtColor(frame, frame, COLOR_BGR2RGB);
		if (!frame.empty())
		{
			QImage image((const uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
			image = image.scaled(ui.label_1->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			ui.label_1->setPixmap(QPixmap::fromImage(image));
		}

		// Write the frame with the detection boxes
		//Mat detectedFrame;
		//frame.convertTo(detectedFrame, CV_8U);
		//if (parser.has("image")) imwrite(outputFile, detectedFrame);
		//else video.write(detectedFrame);

		//imshow(kWinName, frame);
	}
	cap.release(); //释放资源

}
//对于每个帧，提取每个检测到的对象的边界框和 mask
// For each frame, extract the bounding box and mask for each detected object
void QtTask3::postprocess(Mat& frame, const vector<Mat>& outs)
{
	Mat outDetections = outs[0];
	Mat outMasks = outs[1];

	// Output size of masks is NxCxHxW where
	// N - number of detected boxes
	// C - number of classes (excluding background)
	// HxW - segmentation shape
	const int numDetections = outDetections.size[2];
	const int numClasses = outMasks.size[1];

	outDetections = outDetections.reshape(1, outDetections.total() / 7);
	for (int i = 0; i < numDetections; ++i)
	{
		float score = outDetections.at<float>(i, 2);
		if (score > confThreshold)
		{
			// Extract the bounding box
			int classId = static_cast<int>(outDetections.at<float>(i, 1));
			int left = static_cast<int>(frame.cols * outDetections.at<float>(i, 3));
			int top = static_cast<int>(frame.rows * outDetections.at<float>(i, 4));
			int right = static_cast<int>(frame.cols * outDetections.at<float>(i, 5));
			int bottom = static_cast<int>(frame.rows * outDetections.at<float>(i, 6));

			left = max(0, min(left, frame.cols - 1));
			top = max(0, min(top, frame.rows - 1));
			right = max(0, min(right, frame.cols - 1));
			bottom = max(0, min(bottom, frame.rows - 1));
			Rect box = Rect(left, top, right - left + 1, bottom - top + 1);

			// Extract the mask for the object
			Mat objectMask(outMasks.size[2], outMasks.size[3], CV_32F, outMasks.ptr<float>(i, classId));

			// Draw bounding box, colorize and show the mask on the image
			drawBox(frame, classId, score, box, objectMask);
		}
	}
}
//绘制预测的边界框，着色并在图像上显示mask
// Draw the predicted bounding box, colorize and show the mask on the image
void QtTask3::drawBox(Mat& frame, int classId, float conf, Rect box, Mat& objectMask)
{
	//Draw a rectangle displaying the bounding box
	rectangle(frame, Point(box.x, box.y), Point(box.x + box.width, box.y + box.height), Scalar(255, 178, 50), 3);

	//Get the label for the class name and its confidence
	string label = format("%.2f", conf);
	if (!classes.empty())
	{
		CV_Assert(classId < (int)classes.size());
		label = classes[classId] + ":" + label;
	}

	//Display the label at the top of the bounding box
	int baseLine;
	Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
	box.y = max(box.y, labelSize.height);
	rectangle(frame, Point(box.x, box.y - round(1.5*labelSize.height)), Point(box.x + round(1.5*labelSize.width), box.y + baseLine), Scalar(255, 255, 255), FILLED);
	putText(frame, label, Point(box.x, box.y), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 0), 1);

	Scalar color = colors[classId%colors.size()];

	// Resize the mask, threshold, color and apply it on the image
	cv::resize(objectMask, objectMask, Size(box.width, box.height));
	Mat mask = (objectMask > maskThreshold);
	Mat coloredRoi = (0.3 * color + 0.7 * frame(box));
	coloredRoi.convertTo(coloredRoi, CV_8UC3);

	// Draw the contours on the image
	vector<Mat> contours;
	Mat hierarchy;
	mask.convertTo(mask, CV_8U);
	findContours(mask, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	drawContours(coloredRoi, contours, -1, color, 5, LINE_8, hierarchy, 100);
	coloredRoi.copyTo(frame(box), mask);
	Mat result = frame.clone();
	Mat res = frame.clone();
	for (int i = 0; i < result.rows; i++) {
		for (int j = 0; j < result.cols; j++) {
			//根据图像坐标 赋像素值
			result.at<Vec3b>(i, j)[0] = 0;  //black
			result.at<Vec3b>(i, j)[1] = 0;
			result.at<Vec3b>(i, j)[2] = 0;
			res.at<Vec3b>(i, j)[0] = 255; //white
			res.at<Vec3b>(i, j)[1] = 255;
			res.at<Vec3b>(i, j)[2] = 255;
		}
	}
	Mat ROI = res(box);
	ROI.copyTo(result(box), mask);
	imwrite("./imageout/" + std::to_string(number++) + ".jpg", result);

}

