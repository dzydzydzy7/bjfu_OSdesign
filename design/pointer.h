#pragma once
#ifndef POINTER_H
#define POINTER_H
#include <iostream>

class Pointer
{
public:
	Pointer();
	Pointer(int n); //构造单指针
	Pointer(int add,int a);
	~Pointer();
	int get_addr();
	void set_addr(int a);
	int get_next();
	void set_next(int n);

private:
	int addr;
	int next;
};

#endif // POINTER_H