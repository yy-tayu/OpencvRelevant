// PictureSynthesis.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include "opencv.hpp"

using namespace cv;
using namespace std;
enum class Orientation
{
	Up = 0,
	Down,
	Left,
	Right
};
//两张图片合成一张图片
bool twoCompoundOne(Mat _picture1,Mat _picture2,Mat& _out_picture)
{
	int cols = _out_picture.cols;
	int rows = _out_picture.rows;
	//!![1]调整大小
	resize(_picture1, _picture1, cv::Size(cols / 2, rows));
	resize(_picture2, _picture2, cv::Size(cols / 2, rows));
	//!![2]设置ROI 浅拷贝
	Mat left_roi(_out_picture(Rect(0, 0, cols /2, rows)));
	Mat right_roi(_out_picture(Rect(cols / 2, 0, cols /2, rows)));
	//!![3]深拷贝
	_picture1.copyTo(left_roi);
	_picture2.copyTo(right_roi);
	return true;
}

/**
	添加留白 输入：图片、位置、颜色(默认白色)、高度(默认50)
*/
bool addBlankSide(Mat& _out_picture, Orientation _orientation, Scalar color =(0, 0, 0), int _high = 50)
{
	Mat *base_board = nullptr;
	Mat roi; 
	switch (_orientation)
	{
	case Orientation::Up:
		base_board = new Mat(_out_picture.rows + 50, _out_picture.cols, CV_8UC3, color);
		roi = (*base_board)(Rect(0, _high, _out_picture.cols, _out_picture.rows));
		break;
	case Orientation::Down:
		base_board = new Mat(_out_picture.rows + 50, _out_picture.cols, CV_8UC3, color);
		roi = (*base_board)(Rect(0, 0, _out_picture.cols, _out_picture.rows));
		break;
	case Orientation::Left:
		base_board = new Mat(_out_picture.rows, _out_picture.cols + 50, CV_8UC3, color);
		roi = (*base_board)(Rect(_high, 0, _out_picture.cols, _out_picture.rows));
		break;
	case Orientation::Right:
		base_board = new Mat(_out_picture.rows, _out_picture.cols + 50, CV_8UC3, color);
		roi = (*base_board)(Rect(0, 0, _out_picture.cols, _out_picture.rows));
		break;
	default:
		return false;
		break;
	}
	_out_picture.copyTo(roi);
	base_board->copyTo(_out_picture);
	delete base_board;
	base_board = nullptr;
	return true;
}

bool oneCompoundTow(Mat& _out_picture, Mat _src_picture, int _x, int _y, Size _size)
{
	Mat _feature = _src_picture(Rect(_x, _y, _size.width, _size.height)).clone();
	twoCompoundOne(_src_picture, _feature, _out_picture);
	return true;
}

int main()
{
	{
		//!![1]这里创建了 黑色底图
		Mat base_board(960, 1280, CV_8UC3, cv::Scalar(0, 0, 0));
		imshow("测试显示创建的黑图", base_board);
		destroyWindow("测试显示创建的黑图");
		//!![2]两张全景图合成一张
			//!! 1: 准备两张合成图
			/*打开图片*/
			Mat picture1 = imread("../x64/Release/p1.jpg");
			Mat picture2 = imread("../x64/Release/p2.jpg");
			imshow("测试显示图片1", picture1);
			imshow("测试显示图片2", picture2);
			destroyWindow("测试显示图片1");
			destroyWindow("测试显示图片2");
			//!! 2: 改变图片大小,将
			resize(picture1, picture1, cv::Size(1280 / 2, 910));
			resize(picture2, picture2, cv::Size(1280 / 2, 910));
			imshow("测试显示图片1", picture1);
			imshow("测试显示图片2", picture2);
			destroyWindow("测试显示图片1");
			destroyWindow("测试显示图片2");
			//!! 3: 合成
			Mat left_roi = base_board(Rect(0, 0, 1280 / 2, 910));
			Mat right_roi = base_board(Rect(1280 / 2, 0, 1280 / 2, 910));
			picture1.copyTo(left_roi);
			picture2.copyTo(right_roi);
			//addWeighted(picture1, 0.5, picture2, 0.9, 0, base_board); 线性合成
		imshow("测试显示创建的黑图", base_board);
		//waitKey(10000);
		destroyWindow("测试显示创建的黑图");
	}
	{
		/**
		调用封装函数
		[1]创建一个图片大小
		[2]二合一横向合成图片
		[3]添加留白
		*/
		//!![1]创建一个图片大小
		Mat base_board2(910, 1280, CV_8UC3, cv::Scalar(255, 255, 255));
		{
			Mat picture1 = imread("../x64/Release/p1.jpg");
			Mat picture2 = imread("../x64/Release/p2.jpg");
			//!![2]二合一横向合成图片
			twoCompoundOne(picture1, picture2, base_board2);
			//!![3]添加留白
			addBlankSide(base_board2, Orientation::Down);
		}
		imshow("测试合成图", base_board2);
		//waitKey(10000);
		destroyWindow("测试合成图");
	}
	{
		//!![3]一张全景图一张特写合成一张
		Mat base_board3(910, 1280, CV_8UC3, cv::Scalar(255, 255, 255));
		Mat picture1 = imread("../x64/Release/p1.jpg");
		oneCompoundTow(base_board3, picture1, 400, 350, Size(450, 600));
		imshow("测试合成图", base_board3);
		//waitKey(10000);
		destroyWindow("测试合成图");
	}
	Mat result;
	{
		Mat picture1 = imread("../x64/Release/p1.jpg");
		Mat picture2 = imread("../x64/Release/p2.jpg");
		resize(picture1, picture1, cv::Size(1280, 410));
		resize(picture2, picture2, cv::Size(1280, 410));
		result.push_back(picture1);
		result.push_back(picture2);
		Mat testcopy(410, 1280, CV_8UC3, cv::Scalar(255, 255, 255));
		picture1 = testcopy;
		imshow("测试合成图1", result);
	}
	imshow("测试合成图2", result);

	waitKey(10000);

    return 0;
}

