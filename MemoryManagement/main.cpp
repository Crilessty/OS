#include<iostream>
#include<string>
#include<algorithm>
#include<vector>

using namespace std;
#include "define.h"

vector<free_block> fblock;
vector<process> proc;
int num = 1;

//vector中对空闲区域按照空闲空间进行排序 
bool comp_process(process &a, process &b)
{
	return a.start < b.start;
}
bool comp_block(free_block &a, free_block &b)
{
	return a.length < b.length;
}

int main()
{
	string str;
	int n;
	init();
	while (1)
	{
		cin >> str;
		if (str == "new")
		{
			cin >> n;
			create_process(n);
		}
		else if (str == "delete")
		{
			cin >> n;
			
			free_process(n);
		}
		else if (str == "exit")
			exit(0);
		else
			continue;
		show();
	}
	/*
	create_process(200);
	create_process(200);
	create_process(200);
	free_process(2);
	create_process(300);
	create_process(50);
	show();
	*/
	system("pause");
	return 0;
}

void init()
{
	free_block fb;
	fb.start = 1;
	fb.length = MEM_SIZE;
	fblock.push_back(fb);
	cout << "command:\nnew (size)   :create new process \ndelete (pid) :delete process\nexit         :exit MemoryManagement\n" << endl;
}

void create_process(int size)
{
	int i;
	free_block fb;
	process pro;
	sort(fblock.begin(),fblock.end(),comp_block);
	for (i = 0; i < fblock.size(); i++)
		if (fblock[i].length >= size)
			break;
	if (i == fblock.size())
	{
		cout << "don't have enough memery,please delete some process!" << endl;
		return;
	}
	//记录进程
	pro.length = size;
	pro.pid = num ++;
	pro.start = fblock[i].start;
	proc.push_back(pro);
	//记录新产生的内存块
	fb.length = fblock[i].length - size;
	fb.start = fblock[i].start + size ;
	fblock.push_back(fb);
	//释放原有内存块
	fblock.erase(fblock.begin() + i);
}

void free_process(int num)
{
	int i,j,k;
	free_block fb;
	int left = 0, right = 0;
	for (i = 0; i < proc.size(); i++)
	{
		if (proc[i].pid == num)
			break;
	}
	if (i == proc.size())
	{
		cout << "pid number is wrong!" << endl;
		return;
	}
	for (j = 0; j < fblock.size(); j++)
	{
		if (fblock[j].start == proc[i].length + proc[i].start)
			right = j;
		if (fblock[j].start + fblock[j].length == proc[i].start)
			left = j;
	}
	if (left == 0 && right == 0)
	{
		fb.length = proc[i].length;
		fb.start = proc[i].start;
		fblock.push_back(fb);
	}
	else if (left == 0 && right != 0)
	{
		fblock[right].start = proc[i].start;
		fblock[right].length += proc[i].length;
	}
	else if (right == 0 && left != 0)
	{
		fblock[left].length += proc[i].length;
	}
	else
	{
		fblock[left].length += proc[i].length + fblock[right].length;
		fblock.erase(fblock.begin() + right);
	}
	proc.erase(proc.begin() + i);
	num--;
}
void show()
{
	int i;
	sort(proc.begin(), proc.end(), comp_process);
	cout << "*************  show  process  **************************\n"<<endl;
	cout << "Address range         " << "pid number"<<endl<<endl;
	for (i = 0; i < proc.size(); i++)
	{
		printf("(%-4d- %-4d)\t\t%-2d\n\n", proc[i].start, proc[i].length + proc[i].start - 1, proc[i].pid);
	}
	cout << "********************************************************\n" << endl;
}
