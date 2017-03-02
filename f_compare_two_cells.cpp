/*f_compare_two_cells函数主要用于比较两个object(cell),并计算出其相似差D值*/
#include<Eigen/Dense>   
#include<complex>         
#include<iostream>

 using namespace std;           
using namespace Eigen;

float f_res(MatrixXcf Zjk, MatrixXcf Zj, float Cavg_Zjk, float Cavg_Zj, float Cmin_Zjk, float Cmin_Zj, float Cmax_Zjk, float Cmax_Zj);//计算D值

float f_compare_two_cells(MatrixXcf Zj, float Cmin_Zj, float Cmax_Zj, float Cavg_Zj, int compare_j, float cells_intensities[][3], float cells_Z[][16])
{   /*作为对比雨区的Zjk向量表示*/
	MatrixXcf Zjk(8, 1);
	for (int Zjk_i = 0; Zjk_i < 8; Zjk_i++) {
		Zjk(Zjk_i, 0)._Val[0] = cells_Z[compare_j][Zjk_i];
		Zjk(Zjk_i, 0)._Val[1] = cells_Z[compare_j][Zjk_i + 8];
	}

    /* 作为对比雨区的降雨量参数*/
	float Cmin_Zjk = cells_intensities[compare_j][0];
	float Cmax_Zjk = cells_intensities[compare_j][1];
	float Cavg_Zjk = cells_intensities[compare_j][2];
	/*算两cell的相似度误差和*/
	float Fres = f_res(Zjk, Zj, Cavg_Zjk, Cavg_Zj, Cmin_Zjk, Cmin_Zj, Cmax_Zjk, Cmax_Zj); 
	return Fres;
}