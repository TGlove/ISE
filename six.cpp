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

Mat step1(Mat Grey, int x) {
	Mat result = Mat::zeros(Grey.size(), CV_8UC1);
	for (int i = 0; i < Grey.rows; i++) {
		for (int j = 0; j < Grey.cols; j++) {
			if (Grey.at<uchar>(i, j) > 0 && Grey.at<uchar>(i, j) < 100) {
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
			if (Grey.at<uchar>(i, j) > 100 && Grey.at<uchar>(i, j) < 200) {
				result.at<uchar>(i, j) = 255;
			}
		}
	}
	return result;
}
// masking ----------------------------------------------------------------

Mat blur(Mat Grey) {
	Mat result = Mat::zeros(Grey.size(), CV_8UC1);
	for (int i = 1; i < Grey.rows - 1; i++) {
		for (int j = 1; j < Grey.cols - 1; j++) {
			result.at<uchar>(i, j) = Grey.at<uchar>(i, j);
			/*Grey.at<uchar>(i, j) = (Grey.at<uchar>(i-1, j-1)+ Grey.at<uchar>(i-1, j)+ Grey.at<uchar>(i-1, j+1)+ Grey.at<uchar>(i, j-1)+ Grey.at<uchar>(i, j)+ Grey.at<uchar>(i, j+1)
				+ Grey.at<uchar>(i+1, j-1)+ Grey.at<uchar>(i+1, j)+ Grey.at<uchar>(i+1, j+1))/9;*/
			int sum = 0;
			for (int k = -1; k < 2; k++) {
				for (int l = -1; l < 2; l++) {
					sum += Grey.at<uchar>(i + k, j + l);
				}
			}
			result.at<uchar>(i, j) = sum / 9;
		}
	}
	return result;
}

// equalize histogram
Mat equalizeHistogram(Mat Grey) {
	Mat result = Mat::zeros(Grey.size(), CV_8UC1);
	float count[256] = { 0 };
	float prob[256] = { 0 };
	float accumul[256] = { 0 };
	int newpixel[256] = { 0 };
	for (int i = 0; i < Grey.rows; i++) {
		for (int j = 0; j < Grey.cols; j++) {
			count[Grey.at<uchar>(i, j)]++;
		}
	}
	//probability
	for (int i = 0; i < 256; i++) {
		prob[i] = count[i] / (Grey.rows*Grey.cols);
	}
	//accumulative probability
	accumul[0] = prob[0];
	for (int i = 1; i < 256; i++) {
		accumul[i] = accumul[i - 1] + prob[i];
		//new pixels
		newpixel[i] = accumul[i] * 256;
	}
	//change to new pixels
	for (int i = 0; i < Grey.rows; i++) {
		for (int j = 0; j < Grey.cols; j++) {
			for (int k = 0; k < 256; k++) {
				if (Grey.at<uchar>(i, j) == k) {
					result.at<uchar>(i, j) = newpixel[k];
				}
			}
		}
	}
	return result;
}

Mat findBlur(Mat blur) {

	return blur;
}

Mat sobelGx(Mat blur, int th) {
	Mat result = Mat::zeros(blur.size(), CV_8UC1);
	int top = 0, down = 0;
	for (int i = 1; i < blur.rows - 1; i++) {
		for (int j = 1; j < blur.cols - 1; j++) {
			top = blur.at<uchar>(i - 1, j - 1)*(-1) + blur.at<uchar>(i, j - 1)*(-2) + blur.at<uchar>(i + 1, j - 1)*(-1);
			down = blur.at<uchar>(i - 1, j + 1) + blur.at<uchar>(i, j + 1) * 2 + blur.at<uchar>(i + 1, j + 1);
			if (abs(top / 4 + down / 4) > th) {
				result.at<uchar>(i, j) = 255;
			}
		}
	}
	return result;
}

Mat sobelGy(Mat blur, int th) {
	Mat result = Mat::zeros(blur.size(), CV_8UC1);
	int left = 0, right = 0;
	for (int i = 1; i < blur.rows - 1; i++) {
		for (int j = 1; j < blur.cols - 1; j++) {
			left = blur.at<uchar>(i - 1, j - 1)*(-1) + blur.at<uchar>(i - 1, j)*(-2) + blur.at<uchar>(i - 1, j + 1)*(-1);
			right = blur.at<uchar>(i + 1, j - 1) + blur.at<uchar>(i + 1, j) * 2 + blur.at<uchar>(i + 1, j + 1);
			if (abs(left / 4 + right / 4) > th) {
				result.at<uchar>(i, j) = 255;
			}
		}
	}
	return result;
}

Mat laplace(Mat blur, int th) {
	Mat result = Mat::zeros(blur.size(), CV_8UC1);
	int re = 0;
	for (int i = 1; i < blur.rows - 1; i++) {
		for (int j = 1; j < blur.cols - 1; j++) {
			re = blur.at<uchar>(i - 1, j) + blur.at<uchar>(i, j - 1) + blur.at<uchar>(i, j)*(-4) + blur.at<uchar>(i, j + 1) + blur.at<uchar>(i + 1, j);
			if (re > 255) {
				result.at<uchar>(i, j) = 255;
			}
			else if (re < 0) {
				result.at<uchar>(i, j) = 0;
			}
			else
			{
				result.at<uchar>(i, j) = re;
			}
		}
	}
	return result;
}

Mat findedge(Mat blur) {
	Mat result = Mat::zeros(blur.size(), CV_8UC1);
	for (int i = 1; i < blur.rows - 1; i++) {
		for (int j = 1; j < blur.cols - 1; j++) {
			int sum1 = 0, sum2 = 0;
			sum1 = (blur.at<uchar>(i - 1, j - 1) + blur.at<uchar>(i, j - 1) + blur.at<uchar>(i + 1, j - 1))/3;
			sum2 = (blur.at<uchar>(i - 1, j + 1) + blur.at<uchar>(i, j + 1) + blur.at<uchar>(i + 1, j + 1)) / 3;
			if (abs(sum1 - sum2) > 50) {
				result.at<uchar>(i, j) = 255;
			}
		}
	}
	return result;
}

// dilation

Mat dialtion(Mat img,int win) {
	Mat result = Mat::zeros(img.size(), CV_8UC1);
	for (int i = win; i < img.rows-win; i++) {
		for (int j = win; j < img.cols-win; j++) {

			for (int k = -win; k < win; k++) {
				for (int l = -win; l < win; l++) {

					if (img.at<uchar>(k, l) = 255) {
						result.at<uchar>(i, j) = 255;
					}
				}
			}
		}
	}
	return result;
}




void main()
{
	Mat img;
	img = imread("C:\\Users\\TP049606\\Desktop\\a.jpg");
	//imshow("image", img);

	Mat Grey = convertToGrey(img);
	//imshow("grey", Grey);

	Mat darker = equalizeHistogram(Grey);
	//imshow("darker", darker);

	Mat blurgrey = blur(Grey);
	//imshow("blurgrey", blurgrey);

	Mat edge = findedge(blurgrey);
	imshow("edge", edge);

	//grey->equalize->blur->find edge->dilation->erosion->segmentation

	Mat dia = dialtion(edge, 3);
	imshow("dia", dia);

	//Mat sobelgx = laplace(blurgrey, 60);
	//imshow("sobelgx", sobelgx);

	//Mat binarisegrey = convertToBinary(Grey);
	//imshow("binarisegrey", binarisegrey);

	//Mat binariseblur = convertToBinary(blurgrey);
	//imshow("binariseblur", binariseblur);

	//Mat st1 = step1(Grey,100);
	//imshow("step1", st1);

	//Mat st2 = step2(Grey);
	//imshow("step2", st2);

	//Mat st3 = step3(Grey);
	//imshow("step3", st3);

	//Mat Grey = convertToGrey(img);
	//imshow("image1", Grey);
	////waitKey(0);

	//Mat binary = convertToBinary(blurgrey);
	//imshow("image2", binary);

	//Mat ibinary = invertGrey(Grey);
	//imshow("image3", ibinary);
	waitKey(0);
}
