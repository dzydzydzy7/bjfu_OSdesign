#pragma once
#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <cstring>
#include "file.h"
//#include "operation.h"
using namespace std;

class Block
{
public:
	Block();
	Block(unsigned long long bm, int c[], int n);
	void bin2ull();
	void ull2bin();
	int add_string(const char str[]);//����ַ�������һ����������ɣ�����ֻдС��242���ַ���
	int add_file(File file, int posi);
	int add_folder(Folder folder, int posi);
	int add_pointer(Pointer p, int posi);
	unsigned long long get_bit_map();
	int* get_bits_map();
	int* get_cont();
	int get_next();
	void set_bits_map(int start, int end);
	void set_next(int n);
	void free_file(int posi);
	void free_dir(int posi);
	void free_pointer(int posi);
	void print();
	int find_file_posi();//�ҵ����ؿ��ڵ�ַ���Ҳ��ķ���0
	int find_folder_posi();
	int find_pointer_posi();
	int link2block(int posi);//�����ǿ��
	~Block() {};

private:
	//char bit_map[8];
	unsigned long long bit_map;
	int bits_map[64];//0Ϊδʹ��
	//char content[244];
	int cont[61];
	int next;//�ֵ�ַ(�������6λ���Ǹ�)
};

#endif // !BLOCK_H
