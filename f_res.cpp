/*2007论文公式(13)
根据cell的矩阵表示计算出 SEmin（Min intensity SE）、SEmax（Max intensity SE）、SEavg（Avg intensity SE）、
Ed（Dilation SE）、SEr（Rotation SE）、SEt（Translation SE）、RSE（Residual SE）、D（Cell penalty）*/
#include<iostream>
#include<Eigen/Core>
#include<Eigen/Dense>
#include<complex>

using namespace std;
using namespace Eigen;

/*调用 f_rjk(),f_FFjk(),f_Bjk(),f_B0jk(),f_ZZj(),f_RSS()等函数,
分别计算rjk,FFjk,Bjk,B0jk,ZZj,RSS等*/
float f_rjk(MatrixXcf Zjkc, MatrixXcf Zjc);
float f_FFjk(MatrixXcf Zjkc, MatrixXcf Zjc);
MatrixXcf f_Bjk(MatrixXcf Zjk_avg, MatrixXcf Zj_avg, float Rjk, float FFjk);
complex<float> f_b0jk(MatrixXcf Zjk, MatrixXcf Zj, float Rjk, float  FFjk);
MatrixXcf f_ZZj(MatrixXcf Bjk, float Rjk, float FFjk, MatrixXcf Zjk);
float f_RSS(MatrixXcf ZZj, MatrixXcf Zj);

float f_res(MatrixXcf Zjk, MatrixXcf Zj, float Cavg_Zjk, float Cavg_Zj, float  Cmin_Zjk, float Cmin_Zj, float Cmax_Zjk, float Cmax_Zj)
{
	int len_Zjk_row = Zjk.rows();             //计算Zjk矩阵的长度
	int len_Zj_row = Zj.rows();               //计算Zj的矩阵长度
	complex<float> T_Zjk = Zjk.mean();        //求出Zjk均值，用于后面计算
	complex<float> T_Zj = Zj.mean();           //求出Zj均值，用于后面计算

											   //循环初始化矩阵Zjk_avg和Zj_avg的值为1+0*i（复数形式）
	MatrixXcf Zjk_avg(len_Zjk_row, 1), Zj_avg(len_Zjk_row, 1);
	for (int i = 0; i < len_Zjk_row; i++) {
		Zjk_avg(i, 0)._Val[0] = 1;
		Zjk_avg(i, 0)._Val[1] = 0;
		Zj_avg(i, 0)._Val[0] = 1;
		Zj_avg(i, 0)._Val[1] = 0;
	}
	 
	//2007年公式（9）
	Zjk_avg = Zjk_avg*(len_Zjk_row) / (len_Zjk_row - 1)*T_Zjk;
	Zj_avg = Zj_avg*(len_Zj_row) / (len_Zj_row - 1)*T_Zj;
	//计算Zjkc和Zjc对应论文公式（9）和（10）那一段
	MatrixXcf Zjkc = Zjk - Zjk_avg;
	MatrixXcf Zjc = Zj - Zj_avg;

	//计算Rjk、FFjk、Bjk和B0jk(2007年论文公式(8))；ZZj(2007年论文公式(7))
	float Rjk = f_rjk(Zjkc, Zjc);
	float FFjk = f_FFjk(Zjkc, Zjc);
	MatrixXcf Bjk = f_Bjk(Zjk_avg, Zj_avg, Rjk, FFjk);
	complex<float> B0jk = f_b0jk(Zjk, Zj, Rjk, FFjk);
	MatrixXcf ZZj = f_ZZj(Bjk, Rjk, FFjk, Zjk);

	//计算SEd、SEr、SEt、RSE  对应论文中公式（13）
	float SEmin = (Cmin_Zj - Cmin_Zjk)*(Cmin_Zj - Cmin_Zjk);
	float SEmax = (Cmax_Zj - Cmax_Zjk)*(Cmax_Zj - Cmax_Zjk);
	float SEavg = (Cavg_Zj - Cavg_Zjk)*(Cavg_Zj - Cavg_Zjk);

	//计算SEmin、SEmax、SEavg、SEd、SEr、SEt、RSE  对应论文中公式（14）
	float SEd = Rjk*Rjk;
	float SEr = FFjk*FFjk;
	float SEt = abs(B0jk*B0jk);

	//RSS既是RSE对应着2007论文中公式（10）
	float RSE = f_RSS(ZZj, Zj);

	//计算D, 权重按照公式依次是(w1, w2, w3, w4, w5, w6, w7)  对应2010论文中公式（3）
	float w1 = 0.2, w2 = 0.2, w3 = 0.2, w4 = 0.2, w5 = 0.1, w6 = 0.05, w7 = 0.05;
	float D = w1*sqrt(RSE) + w2*(SEavg)+w3*(SEmin)+w4*(SEmax)+w5 * 100 * abs(1 - SEd) + w6 * 100 * SEr + w7*sqrt(SEt);

	//将结果全部返回Res()
	/*MatrixXf Res(1, 8);
	Res << SEmin, SEmax, SEavg, SEd, SEr, SEt, RSE, D;
	return Res;*/
	return D;
}