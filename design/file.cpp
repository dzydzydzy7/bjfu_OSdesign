#include "file.h"

Folder::Folder()
{
	name[0] = '\0';
}

Folder::Folder(const char n[], int p_addr, int c_addr, int f_addr)
{
	strcpy(name, n);
	Pointer p1(p_addr);
	Pointer p2(c_addr);
	Pointer p3(f_addr);
	parent_folder = p1;
	child_folder = p2;
	files = p3;
}

Folder::Folder(const char n[], Pointer p_folder, Pointer c_folder, Pointer f)
{
	strcpy(name, n);
	parent_folder = p_folder;
	child_folder = c_folder;
	files = f;
}

Folder::Folder(const char n[], int p_addr, int p_next, int c_addr, int c_next, int f_addr, int f_next)
{
	strcpy(name, n);
	Pointer p1(p_addr, p_next);
	Pointer p2(c_addr, c_next);
	Pointer p3(f_addr, f_next);
	parent_folder = p1;
	child_folder = p2;
	files = p3;
}

Folder::~Folder()
{
}

char * Folder::get_name()
{
	return name;
}

Pointer Folder::get_parent_floder()
{
	return parent_folder;
}

Pointer Folder::get_child_floder()
{
	return child_folder;
}

Pointer Folder::get_files()
{
	return files;
}

void Folder::set_child_next(int next)
{
	child_folder.set_next(next);
}

void Folder::set_files_next(int next)
{
	files.set_next(next);
}

void Folder::set_parent_next(int next)
{
	parent_folder.set_next(next);
}

void Folder::print()
{
	cout << name << "\n" << parent_folder.get_next() << " " << child_folder.get_next() << " " << files.get_next() << endl;
}

File::File()
{
	first_block = 0;
	folder = 0;
	mod = 0;
}

File::File(const char n[], int b, int f)
{
	strcpy(name, n);
	first_block = b;
	folder = f;
	mod = 0;
}

File::File(const char n[], int b, int f, int m)
{
	strcpy(name, n);
	first_block = b;
	folder = f;
	mod = 0;
}

char * File::get_name()
{
	return name;
}

int File::get_block()
{
	return first_block;
}

int File::get_folder()
{
	return folder;
}

void File::set_folder(int f)
{
	folder = f;
}

void File::set_block(int b)
{
	first_block = b;
}

void File::set_mod(int n)
{
	mod = n;
}

int File::get_mod()
{
	return mod;
}

void File::print()
{
	cout << name << "\n" << first_block << " " << folder << endl;
}
