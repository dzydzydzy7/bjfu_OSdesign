#include "big_block.h"

BigBlock::BigBlock()
{
	memset(block_map, 0, sizeof(block_map));
	memset(blocks_map, 0, sizeof(blocks_map));
	blocks_map[0] = 1;
	blocks_map[1] = 1;
	bin2ui();
}

BigBlock::BigBlock(unsigned int ui[])
{
	for (size_t i = 0; i < 128; i++)
	{
		block_map[i] = ui[i];
	}
	ui2bin();
}

BigBlock::~BigBlock()
{
}

void BigBlock::bin2ui()
{
	for (int i = 0; i < 128; i++)
		for (int j = 0; j < 32; j++)
		{
			block_map[i] = (block_map[i] << 1) + blocks_map[i * 32 + j];
		}
}

void BigBlock::ui2bin()
{
	for (int i = 0; i < 128; i++)
	{
		for (int j = 31; j >= 0; j--)
		{
			blocks_map[i*32 + j] = (block_map[i] >> (31 - j)) & 1;
		}
	}
}

void BigBlock::use_block(int n)
{
	blocks_map[n] = true;
	bin2ui();
}

void BigBlock::free_block(int n)
{
	blocks_map[n] = false;
	bin2ui();
}

unsigned int * BigBlock::get_block_map()
{
	return block_map;
}

int BigBlock::find_new_block()
{
	for (int i = 5; i < 4096; i++)
	{
		if (!blocks_map[i])
			return i;
	}
	return 0;
}

