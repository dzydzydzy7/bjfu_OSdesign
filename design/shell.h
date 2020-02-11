#pragma once
#ifndef SHELL_H
#define SHELL_H

#include "operation.h"

extern BigBlock super_block;
extern Block folder_block;
extern Folder now_folder;
extern int now_folder_posi;
extern string username;//用传参代替

void prompt();
vector<string> spilt_cmd(string str);
void interperter(vector<string> str);
void xunil_shell(string str);
void cd(string);
void dir_c(vector<string> str);
void mkdir_c(string str);
void touch_c(string str);
void more_c(vector<string> str);
void del(string str);
void rmdir_c(vector<string> str);
void move_c(vector<string> str);
void copy_c(vector<string> str);
void import_c(vector<string> str);
void export_c(vector<string> str);

#endif // !SHELL_H
