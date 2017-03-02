/*求出复数的模*/
#include<math.h>
#include<Eigen/Core>
using namespace Eigen;
float f_complex_mod(MatrixXcf T)
{
	float a = T(0, 0)._Val[0];   //a存储实部
	float b = T(0, 0)._Val[1];   //b存储虚部
	return sqrt(a*a + b*b);       //返回模(distance)
}