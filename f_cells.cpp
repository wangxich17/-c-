/* f_cells()主要计算出每个雨区cell的(x + i*sin(angle)*y...)的向量表示既是Z,
在计算出每个cell的Z_intensity(Cx, Cy, Cmin, Cmax, Cavg)，Cell的中心(Cx, Cy)和雨水强度最小值最大值以及均值（Cmin, Cmax, Cavg）*/
#include<Eigen/Dense>   
#include<complex>         
#include<iostream>
#include<math.h>
#include <opencv2/opencv.hpp>     

using namespace std;           //命名空间
using namespace Eigen;
using namespace cv;

const int len_row = 128;     //根据二进制文件设置图像的宽和高
const int len_col = 61;
const int file_size = 7808;
const int threshold_value_rain = 25;   //设置降雨量过滤的阈值
const int threshold_value_contours = 6;  //设置轮廓过滤的阈值

MatrixXf f_intensities(char *filename);
MatrixXf f_angle(MatrixXf boundary, float Cx, float Cy);
MatrixXcf f_Z(MatrixXf Res, float Cx, float Cy);

MatrixXcf f_cells(char *filename)
{
	FILE *fp = fopen(filename, "rb");  //以二进制形式打开文件
	MatrixXcf A(1, 1);
	A << (-1, -1);
	if (fp == NULL) {
		cout << "No such file" << endl;
		exit(-1);
	}
	/*以四个字节为单位，依次读取fp所指向的数值，并且通过阈值threshold_value过滤*/
	float temp;
	Mat I0(len_row, len_col, CV_32F), I1(len_row, len_col, CV_32F), I2;
	for (int i = 0; i < file_size; i++) {
		fread(&temp, 4, 1, fp);
		if (temp > threshold_value_rain) {
			I0.at<float>(i / len_col, i%len_col) = temp;
			I1.at<float>(i / len_col, i%len_col) = 255;
		}
		else {
			I0.at<float>(i / len_col, i%len_col) = 0;
			I1.at<float>(i / len_col, i%len_col) = 0;
		}
		fseek(fp, 4L, 1);            //文件指针后移，继续读取数据
	}
	fclose(fp);//关闭文件
			 
	/*提取各个雨区的轮廓，过滤去轮廓较小的雨区和椭圆拟合时长轴与短轴差别过大的雨区*/
	I1.convertTo(I2, CV_8U, 1, 0);
	vector<vector<Point>> contours;
	vector<vector<Point>> contours2;
	vector<Vec4i> hierarchy;
	Point temp_point;
	findContours(I2, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	for (int i = 0; i<contours.size(); i++) {
		if (contours[i].size()>threshold_value_contours) {          //过滤去轮廓较小的雨区
			Mat pointsf;
			Mat(contours[i]).convertTo(pointsf, CV_32F);
			RotatedRect box = fitEllipse(pointsf);
			if (max(box.size.height, box.size.width) > 15 * min(box.size.height, box.size.width))       //过来掉椭圆拟合较为长短轴差别过大的雨区
				continue;
			vector<Point> temp_vector;
			for (int j = 0; j < contours[i].size(); j++) {
				temp_point.x = contours[i][j].x;
				temp_point.y = contours[i][j].y;
				temp_vector.push_back(temp_point);
			}
			contours2.push_back(temp_vector);   //过滤后的轮廓
		}
	}
	MatrixXf intensities = f_intensities(filename).transpose();  //转置
	MatrixXf Z_intensities = intensities;
	int k = intensities.cols();                //每一个雨区的轮廓在同一列，列数，即是雨区数目
	MatrixXcf Z(8, k);
	/*依次计算每个雨区的向量表示Z;*/
	for (int i = 0; i < k; i++) {
		float Cx = intensities(0, i);        //雨区中心坐标
		float Cy = intensities(1, i);
	    /*依次提取每个雨区的轮廓，计算Z向量表示*/
		MatrixXf B(contours2[i].size(), 2);
		for (int j = 0; j < contours2[i].size(); j++) {
			B(j, 0) = contours2[i][j].x;
			B(j, 1) = contours2[i][j].y;
		}
		
		MatrixXf Res = f_angle(B, Cx, Cy);
		MatrixXcf temp = f_Z(Res, Cx, Cy);
		for (int j = 0; j < 8; j++) {
			Z(j, i)._Val[0] = temp(j, 0)._Val[0];
			Z(j, i)._Val[1] = temp(j, 0)._Val[1];
		}
	}
	if (contours2.size() > 0)
		return Z;
	else
		return A;
}