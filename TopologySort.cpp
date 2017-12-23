// TopologySort.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"Course.h"
#include<iostream>
#include<fstream>
#include<cstdio>
#include"NodeList.h"
#include"Topo.h"
using namespace std;
int main()
{
	char flag;
	Topo a1;
	cout << "确定开始排课表？" << endl << "Y/N" << endl;
	cin >> flag;
	switch (flag)
	{
	case 'Y': a1.toposort(); cout << "排课完成" << endl; break;
	default: exit(1);
	}
	cout << "确定显示课表？" << endl << "Y/N" << endl;
	cin >> flag;
	switch (flag)
	{
	case 'Y': a1.show(); break;
	default: exit(2);
	}
    return 0;
}
