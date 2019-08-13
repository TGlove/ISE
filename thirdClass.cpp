#include "pch.h"
#include <iostream>
#include <highgui/highgui.hpp>
#include <core/core.hpp>

using namespace std;
using namespace cv;

Mat convertToGrey(Mat RGB) {
	Mat Grey = Mat::zeros(RGB.size(), CV_8UC1);
	for (int i = 0; i < RGB.rows; i++) {
		for (int j = 0; j < RGB.cols * 3; j += 3) {
			Grey.at<uchar>(i, j / 3) = (RGB.at<uchar>(i, j) + RGB.at<uchar>(i, j + 1) + RGB.at<uchar>(i, j + 2)) / 3;
		}
	}
	return Grey;

}

Mat convertToBinary(Mat Grey) {
	Mat binary = Mat::zeros(Grey.size(), CV_8UC1);
	for (int i = 0; i < Grey.rows; i++) {
		for (int j = 0; j < Grey.cols; j++) {
			if (Grey.at<uchar>(i, j) > 70) {
				binary.at<uchar>(i, j) = 255;
			}
		}
	}
	return binary;
}

Mat invertGrey(Mat binary) {
	Mat ibinary = Mat::zeros(binary.size(), CV_8UC1);
	for (int i = 0; i < binary.rows; i++) {
		for (int j = 0; j < binary.cols; j++) {
			ibinary.at<uchar>(i, j) = 255 - binary.at<uchar>(i, j);
		}
	}
	return ibinary;
}

Mat step1(Mat Grey,int x) {
	Mat result = Mat::zeros(Grey.size(), CV_8UC1);
	for (int i = 0; i < Grey.rows; i++) {
		for (int j = 0; j < Grey.cols; j++) {
			if (Grey.at<uchar>(i, j) > 0 && Grey.at<uchar>(i, j)<100) {
				result.at<uchar>(i, j) = Grey.at<uchar>(i, j);
			}
			else {
				result.at<uchar>(i, j) = x;
			}
		}
	}
	return result;
}

Mat step2(Mat Grey) {
	Mat result = Mat::zeros(Grey.size(), CV_8UC1);
	for (int i = 0; i < Grey.rows; i++) {
		for (int j = 0; j < Grey.cols; j++) {
			if (Grey.at<uchar>(i, j) > 100 && Grey.at<uchar>(i, j) < 20) {
				result.at<uchar>(i, j) = 255;
			}
		}
	}
	return result;
}

Mat step3(Mat Grey) {
	Mat result = Mat::zeros(Grey.size(), CV_8UC1);
	for (int i = 0; i < Grey.rows; i++) {
		for (int j = 0; j < Grey.cols; j++) {
			if (Grey.at<uchar>(i, j) < 100) {
				result.at<uchar>(i, j) = 100;
			}
			if (Grey.at<uchar>(i, j)>100 && Grey.at<uchar>(i, j)<200) {
				result.at<uchar>(i, j) = 255;
			}
		}
	}
	return result;
}
// masking ----------------------------------------------------------------

Mat blur(Mat Grey) {
	for (int i = 1; i < Grey.rows-1; i++) {
		for (int j = 1; j < Grey.cols-1; j++) {
			/*Grey.at<uchar>(i, j) = (Grey.at<uchar>(i-1, j-1)+ Grey.at<uchar>(i-1, j)+ Grey.at<uchar>(i-1, j+1)+ Grey.at<uchar>(i, j-1)+ Grey.at<uchar>(i, j)+ Grey.at<uchar>(i, j+1)
				+ Grey.at<uchar>(i+1, j-1)+ Grey.at<uchar>(i+1, j)+ Grey.at<uchar>(i+1, j+1))/9;*/
			int sum = 0;
			for (int k = -1; k < 2; k++) {
				for (int l = -1; l < 2; l++) {
					sum += Grey.at<uchar>(i + k, j + l);
				}
			}
			Grey.at<uchar>(i, j) = sum / 9;
		}
	}
	return Grey;
}


void main()
{
	Mat img;
	img = imread("C:\\Users\\TP049606\\Desktop\\a.jpg");
	imshow("image", img);

	Mat Grey = convertToGrey(img);
	//imshow("grey", Grey);

	Mat blurgrey = blur(Grey);
	//imshow("blur", blurgrey);

	Mat binarisegrey = convertToBinary(Grey);
	imshow("binarisegrey", binarisegrey);

	Mat binariseblur = convertToBinary(blurgrey);
	imshow("binariseblur", binariseblur);

	//Mat st1 = step1(Grey,100);
	//imshow("step1", st1);

	//Mat st2 = step2(Grey);
	//imshow("step2", st2);

	//Mat st3 = step3(Grey);
	//imshow("step3", st3);

	//Mat Grey = convertToGrey(img);
	//imshow("image1", Grey);
	////waitKey(0);

	//Mat binary = convertToBinary(Grey);
	//imshow("image2", binary);

	//Mat ibinary = invertGrey(Grey);
	//imshow("image3", ibinary);
	waitKey(0);
}
