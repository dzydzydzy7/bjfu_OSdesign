#pragma once
#ifndef SEEK_H
#define SEEK_H

#include <iostream>
#include <fstream>
#include "block.h"
#include "big_block.h"
using namespace std;

void init_write();
BigBlock seek_big_block();
void write_big_block(BigBlock b);
Pointer seek_pointer(int posi);
void write_pointer(Pointer p, int posi);
File seek_file(int posi);
void write_file(File file, int posi);
Folder seek_folder(int posi);
void write_folder(Folder folder, int posi);
Block seek_block(int posi);
void write_block(Block block, int posi);

#endif // !SEEK_H