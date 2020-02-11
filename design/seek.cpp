#include "seek.h"
using namespace std;

void init_write()
{
	Block b;
	for (int i = 0; i < 4096; i++)
	{
		write_block(b, i << 6);
	}
}

BigBlock seek_big_block()
{
	fstream disk("disk.dat", ios::in | ios::out | ios::binary);
	disk.seekg(0, ios::beg);
	unsigned int block_map[128];
	disk.read((char*)&block_map, sizeof(block_map));
	disk.close();
	return BigBlock(block_map);
}

void write_big_block(BigBlock b)
{
	fstream disk("disk.dat", ios::in | ios::out | ios::binary);
	disk.seekp(0, ios::beg);
	unsigned int *block_map = b.get_block_map();
	disk.write((char*)block_map, 128 * sizeof(int));
	disk.close();
}

Pointer seek_pointer(int posi)
{
	fstream disk("disk.dat", ios::in | ios::out | ios::binary);
	disk.seekg(posi<<2, ios::beg);
	int addr;
	int next;
	disk.read((char*)&addr, sizeof(int));
	//disk.seekg(posi << 2 + 4, ios::beg);
	disk.read((char*)&next, sizeof(int));
	disk.close();
	Pointer p(addr, next);
	return p;
}

void write_pointer(Pointer p, int posi)
{
	fstream disk("disk.dat", ios::in | ios::out | ios::binary);
	disk.seekp(posi << 2, ios::beg);
	int p_addr = p.get_addr();
	disk.write((char *)&p_addr, sizeof(int));
	//disk.seekp(posi << 2 + 4, ios::beg);
	int p_next = p.get_next();
	disk.write((char *)&p_next, sizeof(int));
	disk.close();
}

File seek_file(int posi)
{
	fstream disk("disk.dat", ios::in | ios::out | ios::binary);
	disk.seekg(posi << 2, ios::beg);
	char name[24];
	int b_addr;
	int f_addr;
	int mod;
	disk.read((char*)&name, sizeof(name));
	disk.read((char*)&b_addr, sizeof(b_addr));
	disk.read((char*)&f_addr, sizeof(f_addr));
	disk.read((char*)&mod, sizeof(mod));
	disk.close();
	return File(name, b_addr, f_addr, mod);
}

void write_file(File file, int posi)
{
	fstream disk("disk.dat", ios::in | ios::out | ios::binary);
	disk.seekp(posi << 2, ios::beg);
	disk.write(file.get_name(), 24 * sizeof(char));
	int b_addr = file.get_block();
	int f_addr = file.get_folder();
	int mod = file.get_mod();
	disk.write((char*)&b_addr, sizeof(int));
	disk.write((char*)&f_addr, sizeof(int));
	disk.write((char*)&mod, sizeof(int));
	disk.close();
}

Folder seek_folder(int posi)
{
	fstream disk("disk.dat", ios::in | ios::out | ios::binary);
	disk.seekg(posi << 2, ios::beg);
	char name[24];
	int p_addr, p_next;
	int c_addr, c_next;
	int f_addr, f_next;
	disk.read((char*)&name, sizeof(name));
	disk.read((char*)&p_addr, sizeof(p_addr));
	disk.read((char*)&p_next, sizeof(p_next));
	disk.read((char*)&c_addr, sizeof(c_addr));
	disk.read((char*)&c_next, sizeof(c_next));
	disk.read((char*)&f_addr, sizeof(f_addr));
	disk.read((char*)&f_next, sizeof(f_next));
	disk.close();
	return Folder(name, p_addr, p_next, c_addr, c_next, f_addr, f_next);
}

void write_folder(Folder folder, int posi)
{
	fstream disk("disk.dat", ios::in | ios::out | ios::binary);
	disk.seekp(posi << 2, ios::beg);
	disk.write(folder.get_name(), 24 * sizeof(char));
	int p_addr = folder.get_parent_floder().get_addr();
	int p_next = folder.get_parent_floder().get_next();
	int c_addr = folder.get_child_floder().get_addr();
	int c_next = folder.get_child_floder().get_next();
	int f_addr = folder.get_files().get_addr();
	int f_next = folder.get_files().get_next();
	disk.write((char*)&p_addr, sizeof(int));
	disk.write((char*)&p_next, sizeof(int));
	disk.write((char*)&c_addr, sizeof(int));
	disk.write((char*)&c_next, sizeof(int));
	disk.write((char*)&f_addr, sizeof(int));
	disk.write((char*)&f_next, sizeof(int));
	disk.close();
}

Block seek_block(int posi)
{
	fstream disk("disk.dat", ios::in | ios::out | ios::binary);
	disk.seekg(posi << 2, ios::beg);
	unsigned long long bit_map;
	int cont[61];
	int next;
	disk.read((char*)&bit_map, sizeof(bit_map));
	disk.read((char*)&cont, sizeof(cont));
	disk.read((char*)&next, sizeof(next));
	disk.close();
	return Block(bit_map, cont, next);
}

void write_block(Block block, int posi)
{
	fstream disk("disk.dat", ios::in | ios::out | ios::binary);
	disk.seekp(posi << 2, ios::beg);
	block.bin2ull();
	unsigned long long bit_map = block.get_bit_map();
	disk.write((char*)&bit_map, sizeof(bit_map));
	int *cont = block.get_cont();
	disk.write((char*)cont, 61 * sizeof(int));
	int block_next = block.get_next();
	disk.write((char *)&block_next, sizeof(block.get_next()));
	disk.close();
}
