#include<Eigen/Dense>
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
using namespace Eigen;
const int filenum = 4381;
void f_filename(int dat_i, char filename[]);
void f_getId(int date[][3], int date_i, char Id[30], int num_cells);
MatrixXf f_intensities(char *filename);
MatrixXcf f_cells(char *filename);

void f_hash_cells(int date[][3], char cells_Id[][100], float cells_Z[][16], float cells_intensities[][3], int &count_cells)
{
	for (int dat_i = 0; dat_i < filenum; dat_i++) {
		char filename[100];
		f_filename(dat_i, filename);
		MatrixXf Zj_intensities = f_intensities(filename).transpose();   //转置
		if (Zj_intensities.rows() != 1)                                  //判断是否存在雨区需要查找相似雨区
		{
			MatrixXcf Zj = f_cells(filename);
			for (int kk = 0; kk < Zj_intensities.cols(); kk++) {
				f_getId(date, dat_i, cells_Id[count_cells], kk);/*保存雨区的Id编号*/
																/*保存雨区的向量表示*/
				for (int j = 0; j < 8; j++) {
					cells_Z[count_cells][j] = Zj(j, kk)._Val[0];
					cells_Z[count_cells][j + 8] = Zj(j, kk)._Val[1];
				}
				/*保存雨区的降雨量特征值*/
				cells_intensities[count_cells][0] = Zj_intensities(2, kk);
				cells_intensities[count_cells][1] = Zj_intensities(3, kk);
				cells_intensities[count_cells][2] = Zj_intensities(4, kk);
				count_cells++;
			}
		}
	}
}