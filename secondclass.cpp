#include "pch.h"
#include <iostream>
#include <highgui/highgui.hpp>
#include <core/core.hpp>

using namespace std;
using namespace cv;

Mat convertToGrey(Mat RGB) {
	Mat Grey = Mat::zeros(RGB.size(), CV_8UC1);
	for (int i = 0; i < RGB.rows; i++) {
		for (int j = 0; j < RGB.cols*3; j+=3) {
			Grey.at<uchar>(i, j / 3) = (RGB.at<uchar>(i, j) + RGB.at<uchar>(i, j + 1) + RGB.at<uchar>(i, j + 2)) / 3;
		}
	}
	return Grey;

}

Mat convertToBinary(Mat Grey) {
	Mat binary = Mat::zeros(Grey.size(), CV_8UC1);
	for (int i = 0; i < Grey.rows; i++) {
		for (int j = 0; j < Grey.cols; j ++) {
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

void main()
{
	Mat img;
	img = imread("C:\\Users\\TP049606\\Desktop\\a.jpeg");
	imshow("image", img);
	//waitKey(0);
	
	Mat Grey = convertToGrey(img);
	imshow("image1", Grey);
	//waitKey(0);

	Mat binary = convertToBinary(Grey);
	imshow("image2", binary);

	Mat ibinary = invertGrey(Grey);
	imshow("image3", ibinary);
	waitKey(0);
}
