#include <iostream>
#include <cmath>
#include <string>
using namespace std;
#define N 10               //�ٶ�ϵͳ����������ҵ����Ϊn
#define MEMORY_SIZE 1024	//KB

struct {
	int address;       //�ѷַ�����ʼ��ַ
	int length;        //�ѷַ������ȣ���λΪ�ֽ�
	int flag;            //�ѷ�������Ǽ�����־����0��ʾ����Ŀ��
}used_table[N];          //�ѷ�������

struct {
	int address;       //��������ʼ��ַ
	int length;        //���������ȣ���λΪ�ֽ�
	int flag;            //��������Ǽ���Ŀ��0��ʾ����Ŀ��1��ʾδ����
}free_table[N];          //��������

struct {
	string name;
	int length;
	int flag;
}process[N];

int ut_index = 0; //used table index
int ft_length = 10; //free table length
int p_index = 0; //process index

void init_free_table()
{
	free_table[0].address = 0;
	free_table[0].length = 2;
	free_table[0].flag = 1;
	int start_addr = 2;
	int length = 2;
	for (int i = 1; i < N; i++)
	{
		free_table[i].address = start_addr;
		free_table[i].length = length;
		free_table[i].flag = 1;
		start_addr += length;
		length = length * 2;
	}
}

int getRandomNum(int start, int end)
{
	return (rand() % (end - start + 1)) + start;
}

void init_process()
{
	string str = "process";
	for (int i = 0; i < N; i++)
	{
		process[i].name = str + (char)('0' + i);
		process[i].length = getRandomNum(1, 512);
		process[i].flag = 0;
	}
}

void print_free_table()
{
	cout << "address\t" << "lebgth\t" << "flag" << endl;
	for (int i = 0; i < ft_length; i++)
	{
		printf("%d\t%d\t%d\n", free_table[i].address, free_table[i].length, free_table[i].flag);
	}
}

void print_used_table()
{
	cout << "address\t" << "lebgth\t" << "flag" << endl;
	for (int i = 0; i < ut_index; i++)
	{
		printf("%d\t%d\t%d\n", used_table[i].address, used_table[i].length, used_table[i].flag);
	}
}

void print_process()
{
	cout << "name\t\t" << "length\t" << "flag" << endl;
	for (int i = 0; i < N; i++)
	{
		cout << process[i].name << "\t" << process[i].length << "\t" << process[i].flag << endl;
	}
}

void print_all()
{
	printf("-------------------------------------------------\n");
	printf("free_table:\n");
	print_free_table();
	printf("used_table:\n");
	print_used_table();
	printf("process:\n");
	print_process();
	printf("-------------------------------------------------\n");
}

int worse_fit(int p) //�����i����ҵ
{
	int index = -1;
	int len;
	for (int i = 0; i < ft_length; i++)
	{
		len = free_table[i].length - process[p].length;
		if (len >= 0 && free_table[i].flag)
		{
			index = i;
			break;
		}
	}

	if (index >= 0) process[p].flag = 1;	//��ҵװ������
	else return index;

	used_table[ut_index].address = free_table[index].address;//д�ѷ����
	used_table[ut_index].length = process[p].length;
	used_table[ut_index].flag = p + 1;
	ut_index++;

	if (len != 0)//�Ŀ��б�
	{
		free_table[index].address += process[p].length;
		free_table[index].length -= process[p].length;
	}
	else
	{
		free_table[index].flag = 0;
	}
	return index;
}

void recycle()//�����ڴ�
{
	if (!ut_index)
		return;
	int ut_top = ut_index - 1;
	int index = used_table[ut_top].flag - 1;
	int tail = used_table[ut_top].address + used_table[ut_top].length;//��ҵ��β��ַ
	bool mergeRight = false;
	bool mergeLeft = false;
	//bool haveLeft = true;
	//�ϲ�����
	for (int i = 0; i < N; i++)
	{
		if (tail == free_table[i].address && free_table[i].flag)//���ھ�
		{
			free_table[i].address = used_table[ut_top].address;
			free_table[i].length = used_table[ut_top].length + free_table[i].length;
			free_table[i].flag = 1;
			mergeRight = true;
		}
		//if (used_table[ut_top].address == pow(2, i)) haveLeft = false;
	}

	for (int i = 0; i < N; i++)
	{
		//���ھ�
		if (free_table[i].address + free_table[i].length == used_table[ut_top].address && mergeRight)
		{
			if (mergeRight) {
				free_table[i].length += free_table[i + 1].length;
				free_table[i + 1].flag = 1;
				for (int j = i + 1; j < ft_length - 1; j++)
				{
					free_table[j].address = free_table[j + 1].address;
					free_table[j].length = free_table[j + 1].length;
					free_table[j].flag = free_table[j + 1].flag;
				}
				ft_length--;
				mergeLeft = true;
			}
			else {
				free_table[i].length = used_table[ut_top].length + free_table[i].length;
				free_table[i].flag = 1;
				mergeLeft = true;
			}
		}
	}

	//���Ҷ�û��
	if (!mergeLeft && !mergeRight)
	{
		free_table[ft_length].address = used_table[ut_top].address;
		free_table[ft_length].length = used_table[ut_top].length;
		free_table[ft_length].flag = used_table[ut_top].flag;
		ft_length++;
	}

	//�����ѷ����
	ut_index--;
	process[index].flag = 2;
}

int main()
{
	init_free_table();
	init_process();
	print_all();
	while (p_index < N)
	{
		if (getRandomNum(0, 1))
		{
			if (worse_fit(p_index) != -1)
				p_index++;
		}
		else
			recycle();
		print_all();
	}
	return 0;
}