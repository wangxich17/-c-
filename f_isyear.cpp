#include<iostream>
using namespace std;
int f_isyear(int year)
{
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
		return 1;
	}
	else
		return 0;
}