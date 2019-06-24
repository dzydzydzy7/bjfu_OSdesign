#include "shell.h"

void prompt()
{
	cout << "[" << username << "@Xunil ";
	getwd();
	cout << "]" << "# ";
}

vector<string> spilt_cmd(string str)
{
	vector<string> vec_str;
	string ss = "";
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == ' ') {
			vec_str.push_back(ss);
			ss = "";
		}
		else ss += str[i];
	}
	vec_str.push_back(ss);
	return vec_str;
}

void interperter(vector<string> str)
{
	if (str.size() == 0)return;
	if (str[0] == "cd") {
		if (str.size() > 1)cd(str[1]);
		else cout << "命令不完整" << endl;
	}
	else if (str[0] == "dir") {
		dir_c(str);
	}
	else if (str[0] == "mkdir") {
		if (str.size() > 1)mkdir_c(str[1]);
		else cout << "命令不完整" << endl;
	}
	else if (str[0] == "touch") {
		if (str.size() > 1)touch_c(str[1]);
		else cout << "命令不完整" << endl;
	}
	else if (str[0] == "more") {
		if (str.size() > 1)more_c(str);
		else cout << "命令不完整" << endl;
	}
	else if (str[0] == "del") {
		if (str.size() > 1)del(str[1]);
		else cout << "命令不完整" << endl;
	}
	else if (str[0] == "rmdir") {
		if (str.size() > 1)rmdir_c(str);
		else cout << "命令不完整" << endl;
	}
	else if (str[0] == "move") {
		if (str.size() > 3)move_c(str);
		else cout << "命令不完整" << endl;
	}
	else if (str[0] == "copy") {
		if (str.size() > 3)copy_c(str);
		else cout << "命令不完整" << endl;
	}
	else if (str[0] == "import") {
		if (str.size() > 2)import_c(str);
		else cout << "命令不完整" << endl;
	}
	else if (str[0] == "export") {
		if (str.size() > 2)export_c(str);
		else cout << "命令不完整" << endl;
	}
	else if (str[0] == "exit") {
		exit(0);
	}
	else cout << "\"" << str[0] << "\"" << "不是内部或外部命令" << endl;
}

void xunil_shell(string str)
{
	interperter(spilt_cmd(str));
}

void cd(string str)
{
	string ss = "";
	vector<string> route;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '/') {
			route.push_back(ss);
			ss = "";
		}
		else ss += str[i];
	}
	route.push_back(ss);

	int store_posi = now_folder_posi;
	if (route[0] == "") {
		now_folder_posi = 130;
		now_folder = seek_folder(130);
	}
	bool isOK = true;
	for (size_t i = 0; i < route.size(); i++)
	{
		if (route[i] == "..")
			cd_out();
		else if (route[i] == "." || route[i] == "")continue;
		else {
			char c[24];
			int j = 0;
			for (; j < route[i].size(); j++)
			{
				c[j] = route[i][j];
			}
			c[j] = '\0';
			if (!cd_in(c)) {
				isOK = false;
				break;
			}
		}
	}
	if (!isOK) {
		now_folder_posi = store_posi;
		now_folder = seek_folder(now_folder_posi);
		cout << "路径不正确" << endl;
	}
}

void dir_c(vector<string> str)
{
	if (str.size() < 2){
		dir(now_folder);
	}
	else if (str.size() == 2 && (str[1] == "-S" || str[1] == "-s")) {
		cout << "|-" << now_folder.get_name() << endl;
		dir(now_folder, 0);
	}
	else if (str.size() == 3 && (str[1] == "-F" || str[1] == "-f")){
		cout << "|-" << now_folder.get_name() << endl;
		dir(now_folder, 0, str[2]);
	}
	else cout << "\"" << str[1] << "\"" << "不是参数" << endl;
}

void mkdir_c(string str)
{
	char c[24];
	int i = 0;
	for (; i < str.size(); i++)
	{
		c[i] = str[i];
	}
	c[i] = 0;
	mkdir(c);
}

void touch_c(string str)
{
	char c[24];
	int i = 0;
	for (; i < str.size(); i++)
	{
		c[i] = str[i];
	}
	c[i] = 0;
	touch(c);
}

void more_c(vector<string> str)
{
	if (str.size() == 3 && str[1] == "-v") {
		char c[24];
		int i = 0;
		for (; i < str[2].size(); i++)
		{
			c[i] = str[2][i];
		}
		c[i] = 0;
		more_v(c);
	}
	else if (str.size() == 2) {
		char c[24];
		int i = 0;
		for (; i < str[1].size(); i++)
		{
			c[i] = str[1][i];
		}
		c[i] = 0;
		more(c);
	}
	else cout << "\"" << str[1] << "\"" << "不是参数" << endl;
}

void del(string str)
{
	char c[24];
	int i = 0;
	for (; i < str.size(); i++)
	{
		c[i] = str[i];
	}
	c[i] = 0;
	del_file(c);
}

void rmdir_c(vector<string> str)
{
	if (str.size() == 3 && str[1] == "-r") {
		char c[24];
		int i = 0;
		for (; i < str[2].size(); i++)
		{
			c[i] = str[2][i];
		}
		c[i] = 0;
		rmdir_rf(c);
	}
	else if (str.size() == 2) {
		char c[24];
		int i = 0;
		for (; i < str[1].size(); i++)
		{
			c[i] = str[1][i];
		}
		c[i] = 0;
		rmdir(c);
	}
	else cout << "\"" << str[1] << "\"" << "不是参数" << endl;
}

void move_c(vector<string> str)
{
	char c[24], d[25];
	int i = 0;
	for (; i < str[2].size(); i++)
	{
		c[i] = str[2][i];
	}
	c[i] = 0;
	i = 0;
	for (; i < str[3].size(); i++)
	{
		d[i] = str[3][i];
	}
	d[i] = 0;
	if (str[1] == "-f")
	{
		move_f(c, d);
	}
	else if (str[1] == "-d")
	{
		move_folder(c, d);
	}
	else cout << "\"" << str[1] << "\"" << "不是参数" << endl;
}

void copy_c(vector<string> str)
{
	char c[24], d[25];
	int i = 0;
	for (; i < str[2].size(); i++)
	{
		c[i] = str[2][i];
	}
	c[i] = 0;
	i = 0;
	for (; i < str[3].size(); i++)
	{
		d[i] = str[3][i];
	}
	d[i] = 0;
	if (str[1] == "-f")
	{
		copy_f(c, d);
	}
	else if (str[1] == "-d")
	{
		copy_folder(c, d);
	}
	else cout << "\"" << str[1] << "\"" << "不是参数" << endl;
}

void import_c(vector<string> str)
{
	if (str[1].size() > 23) {
		cout << "文件名过长" << endl;
		return;
	}
	char c[24];
	int i = 0;
	for (; i < str[1].size(); i++)
	{
		c[i] = str[1][i];
	}
	c[i] = 0;
	import_file(c, str[2]);
}

void export_c(vector<string> str)
{
	if (str[1].size() > 23) {
		cout << "文件名过长" << endl;
		return;
	}
	char c[24];
	int i = 0;
	for (; i < str[1].size(); i++)
	{
		c[i] = str[1][i];
	}
	c[i] = 0;
	export_file(c, str[2]);
}
