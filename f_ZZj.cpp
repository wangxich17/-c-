/*2007论文公式（7）*/
#include<iostream>
#include<Eigen/Core>
#include<Eigen/Dense>
#include<complex>

using namespace std;
using namespace Eigen;

MatrixXcf f_ZZj(MatrixXcf Bjk, float Rjk, float FFjk, MatrixXcf Zjk)
{
	complex<float> T_FFjk(0, FFjk);  //将FFjk转化为复数，以用于公式计算
	MatrixXcf ZZj = Bjk + Rjk*exp(T_FFjk)*Zjk;  //按照公式计算
	return ZZj;
}