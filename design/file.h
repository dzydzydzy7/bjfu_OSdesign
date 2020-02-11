#pragma once
#ifndef FILE_H
#define FILE_H
#include <string>
#include <cstring>
#include "pointer.h"

using namespace std;

class Folder
{
public:
	Folder();
	Folder(const char n[], int p_addr, int c_addr, int f_addr);
	Folder(const char n[], Pointer p_folder, Pointer c_folder, Pointer f);
	Folder(const char n[], int p_addr, int p_next, int c_addr, int c_next, int f_addr, int f_next);
	~Folder();
	char* get_name();
	Pointer get_parent_floder();
	Pointer get_child_floder();
	Pointer get_files();
	void set_child_next(int next);
	void set_files_next(int next);
	void set_parent_next(int next);
	void print();

private:
	char name[20];
	Pointer parent_folder;
	Pointer child_folder;
	Pointer files;
};

class File
{
public:
	File();
	File(const char n[], int b, int f);
	File(const char n[], int b, int f, int m);
	char *get_name();
	int get_block();
	int get_folder();
	void set_folder(int f);
	void set_block(int b);
	void set_mod(int n);
	int get_mod();
	void print();
private:
	char name[20];
	int first_block;//�ǿ�ţ����ǵ�ַ�ţ�
	int folder;
	int mod; //0(00) rw 1(01)r  2(10)w  4(11)hide
};

//���ࣺ
//���ԣ�λʾͼ�����ݣ���һ����ĵ�ַ

//����дһ����������һ���ļ���������

//һ�н��ļ���ָ�����һ���ļ��������ļ�������ļ�����һ���ļ�����Ϊ��Щ���Ǽ�¼ʽ�ģ�

#endif // FILE_H
