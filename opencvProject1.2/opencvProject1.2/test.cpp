#include <opencv.hpp>
#include<iostream>
#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;
int H, S, V;

void GetHSV(Mat &image, int div = 64) {

	int nl = image.rows; //行数
	int nc = image.cols; //列数
						 //cout << nl << " " << nc << endl;
	for (int j = 0; j < nl; j++)
	{
		for (int i = 0; i < nc; i++)
		{
			H = image.at<Vec3b>(j, i)[0];
			S = image.at<Vec3b>(j, i)[1];
			V = image.at<Vec3b>(j, i)[2];
			//cout << "H=" << H << "S=" << S << "V=" << V << endl;
			//H 141  153  147  151 145
			//S	38    45   34   50  35
			//V 45    67   59   63  68
			if ((H >= 95) && (H <= 155) && (S >= 43) && (S <= 210) && (V >= 46) && (V <= 85))//检测黄色
			{
				image.at<Vec3b>(j, i)[0] = 0;
				image.at<Vec3b>(j, i)[1] = 240;
				image.at<Vec3b>(j, i)[2] = 120;
			}

		}              
	}
}

int main()
{

	VideoCapture capture(0);
	Mat frame, hsv;  //定义一个Mat变量，用于存储每一帧的图像
	frame = imread("oil.jpg");
	cvtColor(frame, hsv, COLOR_BGR2HSV);
	GetHSV(hsv);
	cvtColor(hsv, hsv, COLOR_HSV2BGR);
	imwrite("oil2.jpg", hsv);
	imshow("转换图", hsv);  //显示
	imshow("原图", frame);  //显示当前帧
	waitKey();
	return 0;
}