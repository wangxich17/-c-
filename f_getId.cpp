/*获取雨区的Id编号*/
#include<string.h>
#include<iostream>
using namespace std;
void f_getId(int date[][3], int date_i, char Id[100], int num_cells)
{
	char Id0[10], Id1[10], Id3[10];
	_itoa(date[date_i][0], Id0, 10);
	strcpy(Id, Id0);
	for (int jj = 1; jj < 3; jj++) {
		if (date[date_i][jj] < 10) {
			strcat(Id, "0");
		}
		_itoa(date[date_i][jj], Id1, 10);
		strcat(Id, Id1);
	}
	strcat(Id, "_");
	_itoa(num_cells, Id3, 10);
	strcat(Id, Id3);
}