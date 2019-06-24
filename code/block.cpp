#include "block.h"

Block::Block()
{
	next = 0;
	bit_map = 0;
	for (size_t i = 0; i < 64; i++)
	{
		bits_map[i] = 0;
	}
	for (size_t i = 0; i < 61; i++)
	{
		cont[i] = 0;
	}
	bits_map[0] = 1;
	bits_map[1] = 1;
	bin2ull();
}

//Block::Block(unsigned long long bm, char c[], int n)
//{
//	bit_map = bm;
//	strcpy(content, c);
//	for (size_t i = 0; i < 61; i++)
//		cont[i] = content[i * 4 + 0] << 24 | content[i * 4 + 1] << 16 | content[i * 4 + 2] << 8 | content[i * 4 + 3];
//	next = n;
//	ull2bin();
//}

Block::Block(unsigned long long bm, int c[], int n)
{
	bit_map = bm;
	for (size_t i = 0; i < 61; i++)
	{
		cont[i] = c[i];
	}
	next = n;
	ull2bin();
}

void Block::bin2ull()
{
	unsigned long long c;
	c = 0;
	for (size_t j = 0; j < 64; j++)
	{
		c = (c << 1) + bits_map[j];
	}
	bit_map = c;
}

void Block::ull2bin()
{
	int count = 0;
	for (int i = 7; i >= 0; i--)
	{
		for (int j = 7; j >= 0; j--)
		{
			bits_map[i * 8 + j] = (bit_map >> count++) & 1;
		}
	}
}

int Block::add_string(const char str[])
{
	for (size_t i = 0; i < 61; i++)
		cont[i] = str[i * 4 + 0] | str[i * 4 + 1] << 8 | str[i * 4 + 2] << 16 | str[i * 4 + 3] << 24;
	return 0;
}

int Block::add_file(File file, int posi)
{
	//文件名
	char *name = file.get_name();
	int index = posi - 2;
	for (size_t i = 0; i < 6; i++)
		cont[i + index] = name[i * 4 + 0] | name[i * 4 + 1] << 8 | name[i * 4 + 2] << 16 | name[i * 4 + 3] << 24;
	index += 6;

	//块
	int b_addr = file.get_block();
	cont[index] = b_addr;
	index += 1;

	//文件夹
	int f_addr = file.get_folder();
	cont[index] = f_addr;
	index += 1;

	//mod
	int mod = file.get_mod();
	cont[index++] = mod;

	//操作bits_map
	for (size_t i = posi; i < posi + 9; i++)
		bits_map[i] = 1;
	bin2ull();
	return 0;
}

int Block::add_folder(Folder folder, int posi)
{
	//文件夹名
	char *name = folder.get_name();
	int index = posi - 2;
	for (size_t i = 0; i < 6; i++)
		cont[i + index] = name[i * 4 + 0] | name[i * 4 + 1] << 8 | name[i * 4 + 2] << 16 | name[i * 4 + 3] << 24;
	index += 6;

	//父文件夹
	int p_addr = folder.get_parent_floder().get_addr();
	cont[index++] = p_addr;

	int p_next = folder.get_parent_floder().get_next();
	cont[index++] = p_next;

	//子文件夹
	int c_addr = folder.get_child_floder().get_addr();
	cont[index++] = c_addr;

	int c_next = folder.get_child_floder().get_next();
	cont[index++] = c_next;

	//文件
	int f_addr = folder.get_files().get_addr();
	cont[index++] = f_addr;

	int f_next = folder.get_files().get_next();
	cont[index++] = f_next;

	//操作bit_map
	for (size_t i = posi; i < posi + 12; i++)
		bits_map[i] = 1;
	bin2ull();
	return 0;
}

int Block::add_pointer(Pointer p, int posi)
{
	int index = posi - 2;
	int addr = p.get_addr();
	cont[index++] = addr;
	int next = p.get_next();
	cont[index++] = next;

	bits_map[posi] = 1;
	bits_map[posi + 1] = 1;
	bin2ull();
	return 0;
}

unsigned long long Block::get_bit_map()
{
	return bit_map;
}

int * Block::get_bits_map()
{
	return bits_map;
}

int * Block::get_cont()
{
	return cont;
}

int Block::get_next()
{
	return next;
}

void Block::set_bits_map(int start, int end)
{
	for (int i = start; i < end; i++)
	{
		bits_map[i] = 1;
	}
	bin2ull();
}

void Block::set_next(int n)
{
	next = n;
	bits_map[63] = 1;
	bin2ull();
}

void Block::free_file(int posi)
{
	for (size_t i = posi; i < posi + 9; i++)
	{
		bits_map[i] = 0;
	}
	bin2ull();
}

void Block::free_dir(int posi)
{
	for (size_t i = posi; i < posi + 12; i++)
	{
		bits_map[i] = 0;
	}
	bin2ull();
}

void Block::free_pointer(int posi)
{
	for (size_t i = posi; i < posi + 2; i++)
	{
		bits_map[i] = 0;
	}
	bin2ull();
}

void Block::print()
{
	char *p;
	p = (char*)&cont;
	cout << bit_map << "\n" << p << "\n" << next << endl;
}

int Block::find_file_posi()
{
	for (int i = 0; i < 61; i++)
	{
		if (!bits_map[i])
		{
			int j = i;
			int count = 0;
			while (!bits_map[j])
			{
				count++;
				j++;
			}
			if (count >= 9)
			{
				return i;
			}
			else i = j;
		}
	}
	return 0;
}

int Block::find_folder_posi()
{
	for (int i = 0; i < 61; i++)
	{
		if (!bits_map[i])
		{
			int j = i;
			int count = 0;
			while (!bits_map[j])
			{
				count++;
				j++;
			}
			if (count >= 12)
			{
				return i;
			}
			else i = j;
		}
	}
	return 0;
}

int Block::find_pointer_posi()
{
	for (int i = 0; i < 61; i++)
	{
		if (!bits_map[i] && !bits_map[i + 1])
			return i;
	}
	return 0;
}

int Block::link2block(int block_num)
{
	int posi = block_num << 6;
	next = posi;
	bits_map[63] = 1;
	bin2ull();
	return 0;
}