#pragma once
#ifndef OPERATION_H
#define OPERATION_H

#include "seek.h"
#include <string>
#include <vector>
//#include <conio.h>

extern BigBlock super_block;
extern Block folder_block;
extern Folder now_folder;
extern int now_folder_posi;

//mkdir touch move copy 都要考虑文件文件夹。已存在的问题

void init();//初始化
int find4folder();//找到适用于文件夹的内存空间
int find4file();//找到适用于文件的内存空间
int find4pointer();//找到适用于指针的内存空间
int get_pointer(Pointer &ptr);//引用型初始化指针，返回指针的地址
int get_pointer(Pointer &ptr, int addr);
int get_pointer(Pointer &ptr, int addr, int next);
void getwd();//可能不需要
void get_start();//初始化(相当于开机，不格式化)
void mkdir(const char n[]);//创建文件夹
void touch(const char str[]);//创建文件
bool cd_in(const char str[]);//进入子文件夹
void cd_out();//返回父文件夹
void dir(Folder folder, int deepth);//再包一层，{写当前文件夹的名字;dir();}
void dir(Folder folder);//只列一级
void dir(Folder folder, int deepth, string P);//通配符
int find_file(const char file_name[]);//del_file，more的子操作
int find_folder(const char folder_name[]); //cd_in,rmdir的子操作，找到当前文件夹的子文件夹地址
int find_any_folder(Folder folder, string folder_name);//goto_folder的子操作
void more(const char file_name[]);//之后改bool，找不到文件夹返回false，反之返回true
void more_v_write(int block_num);//more_v的子操作，往文件里写
void del_file_content(int block_num);//more_v，del_file的子操作，block_num是file的first_block，删除的是文件的内容
void more_v(const char file_name[]);//写文件
int get_folder_posi(Folder folder);//del_file，del_folder的子操作，若删除的是当前文件夹下的可不用
bool del_file(const char file_name[]);//命令行的del
bool folder_is_empty(Folder folder);//rmdir的子操作，判断文件夹是否为空
bool rmdir(const char folder_name[]);//-f不确认，-l确认(默认)，-r递归删
//往vector里存要删除的各类地址
void rm_about_folder(Folder folder, vector<int>&nexts, vector<int>&folder_addr, vector<int>&file_addr);
bool rmdir_rf(const char folder_name[]);//递归删除，看起来删了就行，释放不干净也无所谓，太难调了
int goto_folder(const char folder_way[], Folder &folder); //move_f的子操作，从文件的路径得到引用的folder和返回地址
bool move_f(const char file_name[], const char folder_way[]);//移动文件
bool move_folder(const char folder_name[], const char folder_way[]);//移动文件夹
bool copy_f(const char file_name[], const char folder_name[]);//复制文件
bool copy_creat(Folder folder);//递归的复制文件夹
bool copy_folder(const char folder_name[], const char folder_way[]);//复制文件夹
void import_file(const char file_name[], string local_way);
void export_file(const char file_name[], string local_way);
#endif // !OPERATION_H
