#pragma once
#ifndef BIG_BLOCK_H
#define BIG_BLOCK_H

#include <iostream>
#include <cstring>
using namespace std;

class BigBlock
{
public:
	BigBlock();
	BigBlock(unsigned int ui[]);
	~BigBlock();
	void bin2ui();
	void ui2bin();
	void use_block(int n);
	void free_block(int n);
	unsigned int *get_block_map();
	int find_new_block();

private:
	unsigned int block_map[128];
	bool blocks_map[4096];
};
#endif // !BIG_BLOCK_H
