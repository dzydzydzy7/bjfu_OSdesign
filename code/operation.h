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

//mkdir touch move copy ��Ҫ�����ļ��ļ��С��Ѵ��ڵ�����

void init();//��ʼ��
int find4folder();//�ҵ��������ļ��е��ڴ�ռ�
int find4file();//�ҵ��������ļ����ڴ�ռ�
int find4pointer();//�ҵ�������ָ����ڴ�ռ�
int get_pointer(Pointer &ptr);//�����ͳ�ʼ��ָ�룬����ָ��ĵ�ַ
int get_pointer(Pointer &ptr, int addr);
int get_pointer(Pointer &ptr, int addr, int next);
void getwd();//���ܲ���Ҫ
void get_start();//��ʼ��(�൱�ڿ���������ʽ��)
void mkdir(const char n[]);//�����ļ���
void touch(const char str[]);//�����ļ�
bool cd_in(const char str[]);//�������ļ���
void cd_out();//���ظ��ļ���
void dir(Folder folder, int deepth);//�ٰ�һ�㣬{д��ǰ�ļ��е�����;dir();}
void dir(Folder folder);//ֻ��һ��
void dir(Folder folder, int deepth, string P);//ͨ���
int find_file(const char file_name[]);//del_file��more���Ӳ���
int find_folder(const char folder_name[]); //cd_in,rmdir���Ӳ������ҵ���ǰ�ļ��е����ļ��е�ַ
int find_any_folder(Folder folder, string folder_name);//goto_folder���Ӳ���
void more(const char file_name[]);//֮���bool���Ҳ����ļ��з���false����֮����true
void more_v_write(int block_num);//more_v���Ӳ��������ļ���д
void del_file_content(int block_num);//more_v��del_file���Ӳ�����block_num��file��first_block��ɾ�������ļ�������
void more_v(const char file_name[]);//д�ļ�
int get_folder_posi(Folder folder);//del_file��del_folder���Ӳ�������ɾ�����ǵ�ǰ�ļ����µĿɲ���
bool del_file(const char file_name[]);//�����е�del
bool folder_is_empty(Folder folder);//rmdir���Ӳ������ж��ļ����Ƿ�Ϊ��
bool rmdir(const char folder_name[]);//-f��ȷ�ϣ�-lȷ��(Ĭ��)��-r�ݹ�ɾ
//��vector���Ҫɾ���ĸ����ַ
void rm_about_folder(Folder folder, vector<int>&nexts, vector<int>&folder_addr, vector<int>&file_addr);
bool rmdir_rf(const char folder_name[]);//�ݹ�ɾ����������ɾ�˾��У��ͷŲ��ɾ�Ҳ����ν��̫�ѵ���
int goto_folder(const char folder_way[], Folder &folder); //move_f���Ӳ��������ļ���·���õ����õ�folder�ͷ��ص�ַ
bool move_f(const char file_name[], const char folder_way[]);//�ƶ��ļ�
bool move_folder(const char folder_name[], const char folder_way[]);//�ƶ��ļ���
bool copy_f(const char file_name[], const char folder_name[]);//�����ļ�
bool copy_creat(Folder folder);//�ݹ�ĸ����ļ���
bool copy_folder(const char folder_name[], const char folder_way[]);//�����ļ���
void import_file(const char file_name[], string local_way);
void export_file(const char file_name[], string local_way);
#endif // !OPERATION_H
