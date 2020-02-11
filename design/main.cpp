#include <iostream>
#include <fstream>
#include "pointer.h"
#include "seek.h"
#include "block.h"
#include "operation.h"
#include "shell.h"

using namespace std;
BigBlock super_block;
Block folder_block;
Folder now_folder;
int now_folder_posi = 130;
string username = "root";

void test_pointer()
{
	int a = 10;
	Pointer p1(a, 2);
	Pointer p2(a, 0);
	write_pointer(p1, 0);
	write_pointer(p2, 2);

	Pointer p = seek_pointer(2);
	cout << p.get_addr() << " " << p.get_next() << endl;
	Pointer p3 = seek_pointer(0);
	cout << p3.get_addr() << " " << p3.get_next() << endl;
}

void test_block()
{
	Block b;
	b.add_string("asdf");
	write_block(b, 0);
	Block b1 = seek_block(0);
	b1.print();
}

void test_file()
{
	File f("qwer.cpp", 256, 512);
	Block b;
	b.add_file(f, 2);
	write_block(b, 0);
	Block b1;
	b1 = seek_block(0);
	b1.print();
	File f1 = seek_file(2);
	f1.print();
	write_file(f1, 66);
	File f2 = seek_file(66);
	f2.print();
}

void test_folder()
{
	Folder f("root", 0, 256, 512);
	Block b;
	b.add_folder(f, 2);
	write_block(b, 64);
	Block b1;
	b1 = seek_block(64);
	b1.print();
	Folder f1 = seek_folder(66);
	f1.print();
	write_folder(f1, 2);
	Folder f2 = seek_folder(2);
	f2.print();
}

void full_test1()
{
	init();
	get_start();
	mkdir("root");
	mkdir("etc");
	mkdir("temp");
	cd_in("root");
	touch("block.h");
	more_v("block.h");
	more("block.h");
	touch("block.cpp");
	more_v("block.cpp");
	more("block.cpp");
	mkdir("dir1");
	mkdir("dir2");
	cd_in("dir2");
	mkdir("dir3");
	cd_in("dir3");
	touch("main.cpp");
	more_v("main.cpp");
	more("main.cpp");
	cd_out();
	cd_out();
	cd_out();
	cd_in("temp");
	mkdir("dir4");
	mkdir("dir5");
	cd_out();
	dir(now_folder, 0);
	rmdir_rf("temp");
	dir(now_folder, 0);
	cd_in("root");
	move_f("block.h", "../etc");
	cd_out();
	dir(now_folder, 0);
	cd_in("etc");
	more("block.h");
	cd_out();
	move_folder("root", "etc");
	dir(now_folder, 0);
	cd_in("etc");
	cd_in("root");
	more("block.cpp");
	copy_f("block.cpp", "/etc");
	cd_out();
	cd_out();
	dir(now_folder, 0);
	cd_in("etc");
	more("block.cpp");
	copy_folder("root", "/");
	cd_out();
	dir(now_folder, 0);
	cd_in("root");
	cd_in("dir2");
	cd_in("dir3");
	more("main.cpp");
}

int main()
{
	//init();
	get_start();
	//mkdir("root");
	//mkdir("etc");
	//mkdir("temp");
	cd_in("root");
	//touch("block.h");
	
	while (true)
	{	
		prompt();
		string input;
		getline(cin, input);
		super_block = seek_big_block();
		now_folder = seek_folder(now_folder_posi);
		xunil_shell(input);
	}
}