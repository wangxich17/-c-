/*2007论文公式(8)中第3个公式*/
#include<Eigen/Core>
#include<Eigen/Dense>
#include<complex>
#include<iostream>

using namespace Eigen;
using namespace std;

MatrixXcf f_Bjk(MatrixXcf Zjk_avg, MatrixXcf Zj_avg, float Rjk, float FFjk)
{
	complex<float> T_FFjk(0, FFjk);      //将FFjk转化为复数，以用于公式计算
	MatrixXcf Bjk = Zj_avg - Rjk*exp(T_FFjk)*Zjk_avg;   //按照公式计算
	return Bjk;
}