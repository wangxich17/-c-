/*2007论文公式(8)中第2个公式*/
#include<Eigen/Core>
#include<Eigen/Dense>
#include<complex>
#include<math.h>

using namespace Eigen;
using namespace std;
void f_filename(int dat_i, char filename[]);
float f_FFjk(MatrixXcf Zjkc, MatrixXcf Zjc)
{
	MatrixXcf t1, t2, t3;
	t1 = Zjkc.transpose();         //转置
	t2 = t1.conjugate();           //共轭
	t3 = t2*Zjc;                   //相乘
	complex<float> cm(t3(0, 0)._Val[0], t3(0, 0)._Val[1]);  //将MatrixXcf类型 转化为 complex 类型
	return arg(cm);                                     //计算辐角
}