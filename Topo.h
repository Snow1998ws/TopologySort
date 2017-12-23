#pragma once
#include"Course.h"
#include<fstream>
#include<cstdio>
#include"NodeList.h"
#include"AdjListGraphVexNode.h"
#include<queue>
using namespace std;
class Topo
{
protected:
	int vexNum;                             //顶点或者课程的个数
	AdjListGraphVexNode* vexTable;          //邻接链表
	Course* course;                         //课程
	int term[9];                            //各学期的课程数
	int term_number;                        //学期数
	queue<int>* q;                          
	queue<int>* schedule;                   //排课队列
	int *indegree;                          //各节点的入度
	void statindegree();                    //统计各节点的入度
	int FirstAdjVex(int x);                 //返回顶点x的第一个邻接点
	int NextAdjVex(int v1, int v2);         //返回顶点v1相对于v2的下一个邻接点
public:
	Topo();                              
	void Read();                         //从文件中读取课程信息，建立邻接表
	void toposort();                     //由邻接表进行拓扑排序
	void show();                         //显示课表
};

//显示课表
void Topo::show()
{
	for (int i = 1; i <= term_number; i++)
	{
		cout << "第" << i << "学期课程为:" << endl;
		int Size = schedule[i].size();
		for (int j = 0; j < Size; j++)
		{
			int v = schedule[i].front();
			schedule[i].pop();
			cout << course[v].course_name << '\0' << "      ";
		}
		cout << endl;
	}
}

//统计各节点的入度
void Topo::statindegree()
{
	for (int v = 1; v < vexNum + 1; v++)
	{          //将所有节点入度清零
		indegree[v] = 0;
	}
	for (int v1 = 1; v1 < vexNum + 1; v1++)
	{         
		for (int v2 = FirstAdjVex(v1); v2 != -1; v2 = NextAdjVex(v1, v2))
		{
			indegree[v2]++;                               //v2的入度增1
		}
	}
}

//返回顶点x的第一个邻接点
int Topo::FirstAdjVex(int x)
{
	if (vexTable[x].adjLink.head->next == vexTable[x].adjLink.head)
		return -1;                                          //顶点无邻接点返回-1
	else
		return vexTable[x].adjLink.GetItemPtr(1)->data;           //返回下一个邻接点的编号
}

//返回顶点v1相对于v2的下一个邻接点
int Topo::NextAdjVex(int v1, int v2)
{
	int pos = 0;
	Node<int>* temp = vexTable[v1].adjLink.head->next;            
	while (temp->data != v2)
	{
		pos++;
		temp = temp->next;
	}
	if (pos == vexTable[v1].adjLink.Length())                      //若已经是表尾，返回-1
		return -1;
	else
		return temp->next->data;
}

//从文件中读取课程信息，建立邻接表
void Topo::Read()
{
	vexNum = term_number = 0;
	ifstream infile("course_inf.txt");
	char line[100];
	for (int i = 0; i < 9; i++)
		term[i] = 0;
	while (infile.getline(line, 100))
	{
		int line_len = strlen(line);                              //获取一行的字符长度
		if (line[0] == '/' || line[0] == ' ')                     //若首字符是‘\’或空格就换行
			continue;
		else if (line[0] >= '0' && line[0] <= '9')
		{       //行的第一个字符为数字
			for (int i = 0; i < line_len; i++)
			{
				if (line[i] >= '0' && line[i] <= '9')
				{
					//统计各学期的课程数
					term[++term_number] = line[i] - '0';
				}
			}
			q = new queue<int>[term_number + 1];
			schedule = new queue<int>[term_number + 1];
			int allcourse = 0;
			//求所有课程的数目
			for (int i = 1; i <= term_number; i++)
				allcourse += term[i];
			vexTable = new AdjListGraphVexNode[allcourse + 1]();
			course = new Course[allcourse + 1];
			indegree = new int[allcourse + 1];
		}
		else if (line[0] == 'c')
		{         //行的第一个字符为c
			int pos = 0;
			int p, q;
			vexNum++;                //节点数加一
			int temp_num = (line[1] - '0') * 10 + line[2] - '0';           //计算课程编号
			for (pos = 3; ; pos++)
			{
				if (line[pos] == 9 || line[pos] == ' ')                //当前字符为空格或横向制表
					continue;
				else
				{
					p = pos;
					break;
				}
			}
			for (pos = p; ; pos++)
			{
				if (line[pos] == 9 || line[pos] == ' ')              //当前字符为空格或横向制表
				{
					q = pos;
					break;
				}
			}
			for (pos = 0; pos < q - p; pos++)
				course[temp_num].course_name[pos] = line[pos + p];     //取出课程名
			course[temp_num].course_name[pos + 1] = '\0';
			for (pos = q + 1; pos < line_len; pos++)
			{      
				if (line[pos] >= '0' && line[pos] <= '9')
					break;
			}
			for (pos = pos + 1; pos < line_len; pos++)
			{
				if (line[pos] >= '0' && line[pos] <= '9')
				{//取出课程的排课学期
					course[temp_num].term = line[pos] - '0';
					break;
				}
			}
			vexTable[temp_num].date = course[temp_num];                 //节点的date赋值为Course对象
			for (pos = pos + 1; pos < line_len; pos++)
			{
				if (line[pos] == 'c')           
				{    //当前字符为c
					int k = (line[pos + 1] - '0') * 10 + line[pos + 2] - '0';
					vexTable[k].adjLink.Append(temp_num);               //加入邻接表
				}
			}
		}
	}
}

void Topo::toposort()
{
	int i, size0, sizeTerm, Term, v, w;
	statindegree();
	for (v = 1; v < vexNum + 1; v++)
	{
		if (indegree[v] == 0)
		{
			q[course[v].term].push(v);
		}
	}
	//生成课表
	for (Term = 1; Term <= term_number; Term++)
	{
		size0 = q[0].size();             //无特别要求在哪学期开课的课程
		sizeTerm = q[Term].size();       //要求在第term学期开课的课程

		for (i = 1; i <= sizeTerm; i++)
		{
			v = q[Term].front();                 //取出第term学期开课的课程
			q[Term].pop();                       //将第term学期开课的课程出队列
			schedule[Term].push(v);    //将第term学期开课的课程排进课表
			for (w = FirstAdjVex(v); w != -1; w = NextAdjVex(v, w))
			{
				indegree[w]--;           //w节点的入度减一
				if (indegree[w] == 0 && (course[w].term == 0 || course[w].term > Term))      //w节点入度为0并且课程w无学期要求
					q[course[w].term].push(w);                     //课程w入队
			}
		}
		for (i = 1; i <= term[Term] - sizeTerm; i++)
		{
			v = q[0].front();                             //取出队首的数据信息
			q[0].pop();                                   //出队
			schedule[Term].push(v);                       //入队
			for (w = FirstAdjVex(v); w != -1; w = NextAdjVex(v, w))
			{
				indegree[w]--;                             //w的入度减一
				if (indegree[w] == 0 && (course[w].term == 0 || course[w].term > Term))
					q[course[w].term].push(w);                      //w入队
			}
		}
	}
}

Topo::Topo()
{
	Read();
}
