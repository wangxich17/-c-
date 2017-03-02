/*2007论文公式(8)中第1个公式*/
#include<Eigen/Core>
#include<Eigen/Dense>

using namespace Eigen;

float f_complex_mod(MatrixXcf T);  //计算复数的模长

float f_rjk(MatrixXcf Zjkc, MatrixXcf Zjc)
{
	MatrixXcf t1, t2, t3, t4;          //用于中间计算，作为中间变量
	float T_rjk;                       //保存运算结果并返回  
	t1 = Zjkc.transpose();             //Zjkc矩阵转置
	t2 = t1.conjugate();               //t1矩阵共轭
	t3 = t2*Zjc;                       //两个复数矩阵相乘，结果还是复数矩阵
	t4 = t2*Zjkc;                      //两个复数矩阵相乘，结果还是复数矩阵
	T_rjk = f_complex_mod(t3) / f_complex_mod(t4);        //f_complex_mod()函数用于求一个复数的模
	return T_rjk;
}