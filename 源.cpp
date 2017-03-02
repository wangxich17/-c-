#include<iostream>
#include<Eigen/Dense>
#include<fstream>
#include<string.h>
using namespace std;
using namespace Eigen;

const int scale = 80000;
const int filenum = 4381;
int date[filenum + 10][3];
float cells_Z[scale][16];
char cells_Id[scale][100];
float cells_intensities[scale][3];

/*f_hash_date()哈希处文件顺序和日期的映射表，f_hash_cells()得出每一个雨区的编号Id，和雨区的向量表示以及降雨量特征值*/
void f_hash_cells(int date[][3], char cells_Id[][100], float cells_Z[][16], float cells_intensities[][3], int &count_cells);
void f_hash_date(int date[][3]);
void f_savename(int pos, char savename[]);
float f_compare_two_cells(MatrixXcf Zj, float Cmin_Zj, float Cmax_Zj, float Cavg_Zj, int compare_j, float cells_intensities[][3], float cells_Z[][16]);

int main()
{
	f_hash_date(date);
	int count_cells = 0;//count_cells用于统计数据中有多少个雨区
	f_hash_cells(date, cells_Id, cells_Z, cells_intensities, count_cells);

	/*找出历史中与每个雨区相似的雨区的，并且将编号保存在路径savename中*/
	for (int i = count_cells - 1; i >0; i--) {
		
		/*查找相似雨区的雨区参数*/
		MatrixXcf Zj(8, 1); 
	 
		for (int Zj_i = 0; Zj_i < 8; Zj_i++) {
			Zj(Zj_i, 0)._Val[0] = cells_Z[i][Zj_i];
			Zj(Zj_i, 0)._Val[1] = cells_Z[i][Zj_i + 8];
		}
		float Cmin_Zj = cells_intensities[i][0];
		float Cmax_Zj = cells_intensities[i][1];
		float Cavg_Zj = cells_intensities[i][2];

		char savename[100];
		f_savename(i, savename);
		ofstream out(savename);
		out << cells_Id[i] << endl;
		/*遍历所有历史上的雨区查找具有一定相似的雨区*/
		for (int compare_j = i - 1; compare_j >= 0; compare_j--) {
			float res_compare = f_compare_two_cells(Zj, Cmin_Zj, Cmax_Zj, Cavg_Zj, compare_j, cells_intensities, cells_Z);
			if (res_compare< 15) {
				out << cells_Id[compare_j] << endl;
			}
		}
	}
	
	return 0;
}