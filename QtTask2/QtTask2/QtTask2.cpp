#include "QtTask2.h"
#include <fstream>
#include <sstream>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "common.hpp"
#ifdef CV_CXX11
#include <mutex>
#include <thread>
#include <queue>
#endif



using namespace cv;
using namespace dnn;

QtTask2::QtTask2(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(OnClicked()));
	number = 0;
}

void QtTask2::OnClicked() {
	const std::string modelName = "ssd_caffe";
	const std::string zooFile = "models.yml";
	confThreshold = 0.5;
	nmsThreshold = 0.4;
	float scale = 0.007843;
	Scalar mean = cv::Scalar(127.5, 127.5, 127.5);
	bool swapRB = false;
	int inpWidth = 300;
	int inpHeight = 300;
	size_t async = 0;
	CV_Assert("mobilenet_iter_73000.caffemodel");
	std::string modelPath = findFile("mobilenet_iter_73000.caffemodel");
	std::string configPath = findFile("MobileNetSSD_300x300.prototxt");
	std::string file = "object_detection_classes_pascal_voc.txt";
	std::ifstream ifs(file.c_str());
	if (!ifs.is_open())
		CV_Error(Error::StsError, "File " + file + " not found");
	std::string line;
	while (std::getline(ifs, line))
	{
		classes.push_back(line);
	}
	// Load a model.
	Net net = readNet(modelPath, configPath, "");
	net.setPreferableBackend(0);
	net.setPreferableTarget(0);
	std::vector<String> outNames = net.getUnconnectedOutLayersNames();

	cap.open("ch01_20180507080000.mp4");
	Mat frame, blob;
	while (true)
	{
		cap >> frame;
		cv::resize(frame, frame, cv::Size(1280, 960));
		cvtColor(frame, frame, COLOR_BGR2RGB);
		if (!frame.empty())
		{
			QImage image((const uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
			image = image.scaled(ui.label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			ui.label->setPixmap(QPixmap::fromImage(image));
		}
		cvtColor(frame, frame, COLOR_BGR2RGB);
		Size inpSize(inpWidth > 0 ? inpWidth : frame.cols,
			inpHeight > 0 ? inpHeight : frame.rows);
		blobFromImage(frame, blob, scale, inpSize, mean, swapRB, false);

		// Run a model.
		net.setInput(blob);
		if (net.getLayer(0)->outputNameToIndex("im_info") != -1)  // Faster-RCNN or R-FCN
		{
			cv::resize(frame, frame, inpSize);
			Mat imInfo = (Mat_<float>(1, 3) << inpSize.height, inpSize.width, 1.6f);
			net.setInput(imInfo, "im_info");
		}
		std::vector<Mat> outs;
		net.forward(outs, outNames);

		postprocess(frame, outs, net);

		// Put efficiency information.
		std::vector<double> layersTimes;
		double freq = getTickFrequency() / 1000;
		double t = net.getPerfProfile(layersTimes) / freq;
		std::string label = format("Inference time: %.2f ms", t);
		putText(frame, label, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0));
		cvtColor(frame, frame, COLOR_BGR2RGB);
		if (!frame.empty())
		{
			QImage image((const uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
			image = image.scaled(ui.label_2->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			ui.label_2->setPixmap(QPixmap::fromImage(image));
		}
		waitKey(10);
	}
}



void QtTask2::postprocess(Mat& frame, const std::vector<Mat>& outs, Net& net)
{
	static std::vector<int> outLayers = net.getUnconnectedOutLayers();
	static std::string outLayerType = net.getLayer(outLayers[0])->type;

	std::vector<int> classIds;
	std::vector<float> confidences;
	std::vector<Rect> boxes;
	if (outLayerType == "DetectionOutput")
	{
		// Network produces output blob with a shape 1x1xNx7 where N is a number of
		// detections and an every detection is a vector of values
		// [batchId, classId, confidence, left, top, right, bottom]
		CV_Assert(outs.size() > 0);
		for (size_t k = 0; k < outs.size(); k++)
		{
			float* data = (float*)outs[k].data;
			for (size_t i = 0; i < outs[k].total(); i += 7)
			{
				float confidence = data[i + 2];
				if (confidence > confThreshold)
				{
					int left = (int)data[i + 3];
					int top = (int)data[i + 4];
					int right = (int)data[i + 5];
					int bottom = (int)data[i + 6];
					int width = right - left + 1;
					int height = bottom - top + 1;
					if (width * height <= 1)
					{
						left = (int)(data[i + 3] * frame.cols);
						top = (int)(data[i + 4] * frame.rows);
						right = (int)(data[i + 5] * frame.cols);
						bottom = (int)(data[i + 6] * frame.rows);
						width = right - left + 1;
						height = bottom - top + 1;
					}
					classIds.push_back((int)(data[i + 1]) - 1);  // Skip 0th background class id.
					boxes.push_back(Rect(left, top, width, height));
					confidences.push_back(confidence);
				}
			}
		}
	}
	else if (outLayerType == "Region")
	{
		for (size_t i = 0; i < outs.size(); ++i)
		{
			// Network produces output blob with a shape NxC where N is a number of
			// detected objects and C is a number of classes + 4 where the first 4
			// numbers are [center_x, center_y, width, height]
			float* data = (float*)outs[i].data;
			for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
			{
				Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
				Point classIdPoint;
				double confidence;
				minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
				if (confidence > confThreshold)
				{
					int centerX = (int)(data[0] * frame.cols);
					int centerY = (int)(data[1] * frame.rows);
					int width = (int)(data[2] * frame.cols);
					int height = (int)(data[3] * frame.rows);
					int left = centerX - width / 2;
					int top = centerY - height / 2;

					classIds.push_back(classIdPoint.x);
					confidences.push_back((float)confidence);
					boxes.push_back(Rect(left, top, width, height));
				}
			}
		}
	}
	else
		CV_Error(Error::StsNotImplemented, "Unknown output layer type: " + outLayerType);

	std::vector<int> indices;
	dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
	for (size_t i = 0; i < indices.size(); ++i)
	{
		int idx = indices[i];
		Rect box = boxes[idx];
		drawPred(classIds[idx], confidences[idx], box.x, box.y,
			box.x + box.width, box.y + box.height, frame);
	}
}

void QtTask2::drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame)
{
	rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 255, 0));

	std::string label = format("%.2f", conf);
	if (!classes.empty())
	{
		CV_Assert(classId < (int)classes.size());
		label = classes[classId] + ": " + label;
	}

	int baseLine;
	Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

	top = max(top, labelSize.height);
	rectangle(frame, Point(left, top - labelSize.height),
		Point(left + labelSize.width, top + baseLine), Scalar::all(255), FILLED);
	
	Rect rect = Rect(Point(left, top), Point(right, bottom));
	imwrite("./person/" + std::to_string(number) + ".jpg", frame(rect));
	number++;
	putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.5, Scalar());
}

