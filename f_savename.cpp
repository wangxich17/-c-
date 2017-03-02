#include<string.h>
#include<iostream>
using namespace std;

/*获取文件保存路劲*/

void f_savename(int pos, char savename[])
{
	char numtostr[20];
	_itoa(pos, numtostr, 10);

	char savename0[40] = "F:\\fcst_similar_cells\\similar_fcst_";
	strcpy(savename, savename0);
	strcat(savename, numtostr);
	char savename1[10] = ".txt";
	strcat(savename, savename1);
}