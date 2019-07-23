#include "pch.h"
#include <iostream>
#include <highgui/highgui.hpp>
#include <core/core.hpp>

using namespace std;
using namespace cv;

void main()
{
	Mat img;
	img = imread("C:\\Users\\TP049606\\Desktop\\a.jpeg");
	imshow("image", img);
	waitKey(0);
}
