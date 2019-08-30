#include <opencv.hpp>
#include<iostream>
#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;
int H, S, V;

void GetHSV(Mat &image, int div = 64) {

	int nl = image.rows; //����
	int nc = image.cols; //����
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
			if ((H >= 95) && (H <= 155) && (S >= 43) && (S <= 210) && (V >= 46) && (V <= 85))//����ɫ
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
	Mat frame, hsv;  //����һ��Mat���������ڴ洢ÿһ֡��ͼ��
	frame = imread("oil.jpg");
	cvtColor(frame, hsv, COLOR_BGR2HSV);
	GetHSV(hsv);
	cvtColor(hsv, hsv, COLOR_HSV2BGR);
	imwrite("oil2.jpg", hsv);
	imshow("ת��ͼ", hsv);  //��ʾ
	imshow("ԭͼ", frame);  //��ʾ��ǰ֡
	waitKey();
	return 0;
}