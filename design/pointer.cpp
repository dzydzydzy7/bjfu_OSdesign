#include "pointer.h"

Pointer::Pointer()
{
	next = 0;
	addr = 0;
}

Pointer::Pointer(int n)
{
	next = n;
	addr = 0;
}

Pointer::Pointer(int add,int a)
{
	addr = add;
	next = a;
}

Pointer::~Pointer()
{
}

int Pointer::get_addr()
{
	return addr;
}

void Pointer::set_addr(int a)
{
	addr = a;
}

int Pointer::get_next()
{
	return next;
}

void Pointer::set_next(int n)
{
	next = n;
}
