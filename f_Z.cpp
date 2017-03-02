/*以中心(Cx, Cy)为偏角中心点，求出各个偏角（2*pi(L - 1)(/ A - 1)）, L = 0, 1...A - 1)值到边界区域edge
所相交的像素点的坐标。遍历像素点的偏角值，当某个像素相对于中心的偏角alpha和所求偏角angle相差小于pi / 16时，
可考虑使用该位置记录为相应的偏角位置，当有多个位置满足时，使用误差最小的作为替代位置。
当在误差的范围内找不到合适的点来描述偏角anglg时与边界区域的交点时，取所有像素点中到中心点最远的R，
记录该位置为x = Cx + R*sin(angle), y = Cy + R*cos(angle)*/
#include<math.h>
#include<iostream>
#include<Eigen/Core>
#include<Eigen/Dense>
#include<complex>

using namespace std;
using namespace Eigen;
const float PI = 3.14159265358979323846;//圆周率PI

MatrixXcf f_Z(MatrixXf Res, float Cx, float Cy) {
	MatrixXcf Z(8, 1);
	int k, pos = 0;
	float X_Rmax, Y_Rmax, flag, angle, t_min, t_abs, t_cha, tx, ty, t_r, Rmax = Res(0, 3);
	flag = PI / 16;              // 所设置误差(flag)
								 //寻找距离中心点最远的点
	k = Res.rows();
	for (int i = 1; i < k; i++) {
		if (Rmax< Res(i, 3)) {    //更新最远距离
			Rmax = Res(i, 3);
			pos = i;
		}
	}
	X_Rmax = Res(pos, 0);    //最远距离点的坐标x,y
	Y_Rmax = Res(pos, 1);
	//计算采样偏角和距离最适合的点
	for (int i = 0; i < 8; i++) {
		angle = i * 2 * PI / 8.0;       //更新采样偏角
		t_min = 5 * PI;
		for (int j = 0; j < k; j++) {
			t_cha = abs(Res(j, 2) - angle);
			t_abs = abs(t_min - angle);
			if (t_cha<t_abs) {     //更新坐标点
				t_r = Res(j, 3);
				t_min = Res(j, 2);
				tx = Res(j, 0);
				ty = Res(j, 1);
			}
		}

		if (abs(t_min - angle)>flag) {      //判断坐标点是在采样偏角误差范围内，若不在需使用Rmax
			t_r = Rmax;
			t_min = angle;
			tx = cos(angle) + Cx;
			ty = sin(angle) + Cy;
		}
		Z(i, 0)._Val[0] = tx;   //Z中按照顺序存放被选中的(x,y)坐标
		Z(i, 0)._Val[1] = ty;
	}
	return Z;
}