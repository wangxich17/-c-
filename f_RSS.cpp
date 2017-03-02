/*2007论文公式（11）*/
#include<iostream>
#include<Eigen/Core>
#include<Eigen/Dense>
#include<complex>
#include<math.h>

using namespace std;
using namespace Eigen;

float f_RSS(MatrixXcf ZZj, MatrixXcf Zj)
{
	MatrixXcf t1, t2, t3;
	t1 = (ZZj - Zj).transpose();   //先计算Zjk-Zj
	t2 = t1.conjugate();           //求出共轭
	t3 = t2*(ZZj - Zj);            //相乘
	float RSS = t3(0, 0)._Val[0];  // 计算结果
	return RSS;
}