#include "operation.h"

void init()
{
	init_write();
	BigBlock b;
	write_big_block(b);
	Folder main_folder("", 194, 196, 198);
	Block block2;
	block2.add_folder(main_folder, 2);
	write_block(block2, 2 << 6);
	Pointer pp(130, 0);
	Pointer pc;
	Pointer pf;
	Block block3;
	block3.add_pointer(pp, 2);
	block3.add_pointer(pc, 4);
	block3.add_pointer(pf, 6);
	write_block(block3, 3 << 6);

	Block test_2 = seek_block(2 << 6);
	Block test_3 = seek_block(3 << 6);
	Folder mf = seek_folder(130);
	//mf.print();
	int p = block2.find_file_posi();
	//cout << p << endl;
	b.use_block(2);
	b.use_block(3);
	write_big_block(b);
}

int find4folder()
{
	int block_num = 2;
	Block block = seek_block(block_num << 6);
	while (true)
	{
		if (block.find_folder_posi())
			return block.find_folder_posi() + (block_num << 6);
		else
		{
			if (block.get_next())
			{
				block_num = block.get_next() >> 6;
				block = seek_block(block.get_next());
			}
			else {
				int temp_block = block_num;
				block_num = super_block.find_new_block();
				block.set_next(block_num << 6);
				super_block.use_block(block_num);
				write_block(block, temp_block << 6);
				write_big_block(super_block);

				block = seek_block(block.get_next());
			}
		}
	}
}

int find4file()
{
	int block_num = 4;
	Block file_block = seek_block(block_num << 6);
	while (true)
	{
		if (file_block.find_file_posi())
			return file_block.find_file_posi() + (block_num << 6);
		else
		{
			if (file_block.get_next())
			{
				file_block = seek_block(file_block.get_next());
			}
			else {
				block_num = super_block.find_new_block();
				file_block.set_next(block_num << 6);
				super_block.use_block(block_num);

				file_block = seek_block(file_block.get_next());
			}
		}
	}
}

int find4pointer()
{
	int block_num = 3;
	Block pointer_block = seek_block(block_num << 6);
	while (true) {
		if (pointer_block.find_pointer_posi())
			return pointer_block.find_pointer_posi() + (block_num << 6);
		else {
			if (pointer_block.get_next())
				pointer_block = seek_block(pointer_block.get_next());
			else
			{
				block_num = super_block.find_new_block();
				pointer_block.set_next(block_num << 6);
				super_block.use_block(block_num);

				pointer_block = seek_block(pointer_block.get_next());
			}
		}
	}
}

int get_pointer(Pointer &ptr)
{
	int mask = 0b111111;
	int ptr_posi = find4pointer();
	Block ptr_block = seek_block((ptr_posi >> 6) << 6);
	ptr_block.add_pointer(ptr, ptr_posi&mask);
	write_block(ptr_block, (ptr_posi >> 6) << 6);
	super_block.use_block(ptr_posi >> 6);
	return ptr_posi;
}

int get_pointer(Pointer &ptr, int addr)
{
	int mask = 0b111111;
	int ptr_posi = find4pointer();
	Block ptr_block = seek_block((ptr_posi >> 6) << 6);
	ptr.set_addr(addr);
	ptr_block.add_pointer(ptr, ptr_posi&mask);
	write_block(ptr_block, (ptr_posi >> 6) << 6);
	super_block.use_block(ptr_posi >> 6);
	return ptr_posi;
}

int get_pointer(Pointer & ptr, int addr, int next)
{
	int mask = 0b111111;
	int ptr_posi = find4pointer();
	Block ptr_block = seek_block((ptr_posi >> 6) << 6);
	ptr.set_addr(addr);
	ptr.set_next(next);
	ptr_block.add_pointer(ptr, ptr_posi&mask);
	write_block(ptr_block, (ptr_posi >> 6) << 6);
	super_block.use_block(ptr_posi >> 6);
	return ptr_posi;
}

void getwd()
{
	vector<string> str;
	Folder folder = seek_folder(now_folder_posi);
	str.push_back(folder.get_name());
	int parent_fd_posi = seek_pointer(folder.get_parent_floder().get_next()).get_addr();
	while (parent_fd_posi != 130)
	{
		folder = seek_folder(parent_fd_posi);
		parent_fd_posi = seek_pointer(folder.get_parent_floder().get_next()).get_addr();
		str.push_back(folder.get_name());
	}
	for (int i = str.size() - 1; i >= 0; i--)
	{
		cout << "/" << str[i];
	}
}

void get_start()
{
	super_block = seek_big_block();
	folder_block = seek_block(128);
	//folder_block.print();
	now_folder = seek_folder(130);
	//now_folder.print();
	super_block.use_block(2);//主文件夹用的是第二块
	write_big_block(super_block);
	super_block = seek_big_block();
}

void mkdir(const char n[])
{
	int mask = 0b111111;

	//父文件夹，子文件夹，子文件的尾指针
	Pointer parent_ptr;
	Pointer child_ptr;
	Pointer file_ptr;
	int parent_ptr_posi = get_pointer(parent_ptr, now_folder_posi);
	int child_ptr_posi = get_pointer(child_ptr);
	int file_ptr_posi = get_pointer(file_ptr);

	//新建子文件夹
	int new_folder_posi = find4folder();
	//child_ptr.set_addr(new_folder_posi);//需要获得新文件夹的地址 现在改成头插
	Block new_folder_block = seek_block((new_folder_posi >> 6) << 6);
	Folder new_folder(n, parent_ptr_posi, child_ptr_posi, file_ptr_posi);
	new_folder_block.add_folder(new_folder, new_folder_posi&mask);
	write_block(new_folder_block, (new_folder_posi >> 6) << 6);
	super_block.use_block(new_folder_posi >> 6);

	//子文件夹和当前文件夹在一个目录下是，读出同步是必要的
	folder_block = seek_block((now_folder_posi >> 6) << 6);

	//指针指向新建的子文件夹，并头插到当前文件夹的子文件夹指针链表
	Pointer the_ptr;
	int next = now_folder.get_child_floder().get_next();
	int the_ptr_posi = get_pointer(the_ptr, new_folder_posi, next);
	now_folder.set_child_next(the_ptr_posi);
	folder_block.add_folder(now_folder, now_folder_posi&mask);

	write_block(folder_block, (now_folder_posi >> 6) << 6);
	write_big_block(super_block);

	////子文件夹和当前文件夹在一个目录下是，读出同步是必要的
	//folder_block = seek_block((now_folder_posi >> 6) << 6);
	//folder_block.add_folder(now_folder, now_folder_posi);
	//write_block(folder_block, (now_folder_posi >> 6) << 6);
}

void touch(const char str[])
{
	int mask = 0b111111;

	int folder_posi = now_folder_posi;
	int first_block = 0;//0值还是直接分配一个块？还没想好
	//int first_block = super_block.find_new_block();
	int mod = 0;

	//设置文件和写文件
	int file_posi = find4file();
	Block new_file_block = seek_block((file_posi >> 6) << 6);
	File new_file(str, first_block, folder_posi, mod);
	new_file_block.add_file(new_file, file_posi&mask);
	write_block(new_file_block, (file_posi >> 6) << 6);
	super_block.use_block(file_posi >> 6);

	//把文件链接到文件夹
	Pointer the_ptr;
	int next = now_folder.get_files().get_next();
	int the_ptr_posi = get_pointer(the_ptr, file_posi, next);
	now_folder.set_files_next(the_ptr_posi);
	folder_block.add_folder(now_folder, now_folder_posi&mask);

	write_block(folder_block, (now_folder_posi >> 6) << 6);
	write_big_block(super_block);

	/*folder_block = seek_block((now_folder_posi >> 6) << 6);
	folder_block.add_folder(now_folder, now_folder_posi);
	write_block(folder_block, (now_folder_posi >> 6) << 6);*/
}

bool cd_in(const char str[])
{
	int addr = find_folder(str);
	if (!addr)return false;
	folder_block = seek_block((addr >> 6) << 6);
	Folder child_folder = seek_folder(addr);
	now_folder = child_folder;
	now_folder_posi = addr;
	return true;
}

void cd_out()
{
	int addr = now_folder.get_parent_floder().get_next();
	Pointer parent_folder_ptr = seek_pointer(addr);
	int parent_folder_addr = parent_folder_ptr.get_addr();
	folder_block = seek_block((parent_folder_addr >> 6) << 6);
	now_folder = seek_folder(parent_folder_addr);
	now_folder_posi = parent_folder_addr;
}

void dir(Folder folder, int deepth)
{
	Pointer child_folder_ptr = folder.get_child_floder();
	int addr;
	while (child_folder_ptr.get_next() != 0)
	{
		addr = child_folder_ptr.get_addr();
		if (!addr) {
			child_folder_ptr = seek_pointer(child_folder_ptr.get_next());
			continue;
		}
		Folder child_folder = seek_folder(addr);
		for (int i = 0; i <= deepth; i++)cout << "  ";
		cout << "|-" << child_folder.get_name() << endl;
		dir(child_folder, deepth + 1);
		child_folder_ptr = seek_pointer(child_folder_ptr.get_next());
	}
	Pointer files_ptr = folder.get_files();
	while (files_ptr.get_next() != 0)
	{
		addr = files_ptr.get_addr();
		if (!addr) {
			files_ptr = seek_pointer(files_ptr.get_next());
			continue;
		}
		File file = seek_file(addr);
		for (int i = 0; i <= deepth; i++)cout << "  ";
		cout << " -" << file.get_name() << endl;
		files_ptr = seek_pointer(files_ptr.get_next());
	}
}

void dir(Folder folder)
{
	Pointer child_folder_ptr = folder.get_child_floder();
	int addr;
	while (child_folder_ptr.get_next() != 0)
	{
		addr = child_folder_ptr.get_addr();
		if (!addr) {
			child_folder_ptr = seek_pointer(child_folder_ptr.get_next());
			continue;
		}
		Folder child_folder = seek_folder(addr);
		cout << child_folder.get_name() << endl;
		child_folder_ptr = seek_pointer(child_folder_ptr.get_next());
	}
	Pointer files_ptr = folder.get_files();
	while (files_ptr.get_next() != 0)
	{
		addr = files_ptr.get_addr();
		if (!addr) {
			files_ptr = seek_pointer(files_ptr.get_next());
			continue;
		}
		File file = seek_file(addr);
		cout << file.get_name() << endl;
		files_ptr = seek_pointer(files_ptr.get_next());
	}
}

int * buildNext(string P)
{
	int m = P.size(), j = 0;
	int *N = new int[m];
	int t = N[0] = -1;
	while (j < m - 1)
		if (0 > t || P[j] == P[t]) {
			j++; t++;
			if (P[j] == P[t])N[j] = N[t];
			else N[j] = t;
			//N[j] = (P[++j] != P[++t] ? t : N[t]);
		}
		else t = N[t];
	return N;
}

int kmp(string P, string T)
{
	int *next = buildNext(P);
	int n = T.size(), i = 0;
	int m = P.size(), j = 0;
	while (j < m  && i < n)
		if (0 > j || T[i] == P[j])
		{
			i++;  j++;
		}
		else
			j = next[j];
	delete[] next;
	return i - j;
}

void dir(Folder folder, int deepth, string P)
{
	Pointer child_folder_ptr = folder.get_child_floder();
	int addr;
	while (child_folder_ptr.get_next() != 0)
	{
		addr = child_folder_ptr.get_addr();
		if (!addr) {
			child_folder_ptr = seek_pointer(child_folder_ptr.get_next());
			continue;
		}
		Folder child_folder = seek_folder(addr);
		for (int i = 0; i <= deepth; i++)cout << "  ";
		cout << "|-" << child_folder.get_name() << endl;
		dir(child_folder, deepth + 1, P);
		child_folder_ptr = seek_pointer(child_folder_ptr.get_next());
	}
	Pointer files_ptr = folder.get_files();
	while (files_ptr.get_next() != 0)
	{
		addr = files_ptr.get_addr();
		if (!addr) {
			files_ptr = seek_pointer(files_ptr.get_next());
			continue;
		}
		File file = seek_file(addr);
		string t = file.get_name();
		if (kmp(P, t) < t.size())
		{
			for (int i = 0; i <= deepth; i++)cout << "  ";
			cout << " -" << file.get_name() << endl;
		}
		files_ptr = seek_pointer(files_ptr.get_next());
	}
}

int find_file(const char file_name[])
{
	Pointer file_ptr = now_folder.get_files();
	File file;
	int addr;
	while (file_ptr.get_next() != 0)
	{
		addr = file_ptr.get_addr();
		if (!addr)
		{
			file_ptr = seek_pointer(file_ptr.get_next());
			continue;
		}
		file = seek_file(addr);
		if (strcmp(file.get_name(), file_name) == 0)
			return addr;
		file_ptr = seek_pointer(file_ptr.get_next());
	}
	return 0;
}

int find_folder(const char folder_name[])
{
	Pointer folder_ptr = now_folder.get_child_floder();
	int addr;
	bool is_find = false;
	Folder folder;
	while (folder_ptr.get_next() != 0)
	{
		addr = folder_ptr.get_addr();
		if (!addr) {
			folder_ptr = seek_pointer(folder_ptr.get_next());
			continue;
		}
		folder = seek_folder(addr);
		if (strcmp(folder.get_name(), folder_name) == 0) {
			is_find = true;
			break;
		}
		folder_ptr = seek_pointer(folder_ptr.get_next());
	}
	if (!is_find)return 0;
	return addr;
}

int find_any_folder(Folder input_folder, string folder_name)
{
	char name[24];
	int index = 0;
	for (; index < folder_name.size(); index++)
		name[index] = folder_name[index];
	name[index] = '\0';
	Pointer folder_ptr = input_folder.get_child_floder();
	int addr;
	bool is_find = false;
	Folder folder;
	while (folder_ptr.get_next() != 0)
	{
		addr = folder_ptr.get_addr();
		if (!addr) {
			folder_ptr = seek_pointer(folder_ptr.get_next());
			continue;
		}
		folder = seek_folder(addr);
		if (strcmp(folder.get_name(), name) == 0) {
			is_find = true;
			break;
		}
		folder_ptr = seek_pointer(folder_ptr.get_next());
	}
	if (!is_find)return 0;
	return addr;
}

void more(const char file_name[])
{
	if (!find_file(file_name)) {
		cout << "文件不存在" << endl;
		return;
	}
	File file = seek_file(find_file(file_name));
	int content_block_posi = file.get_block();
	Block content_block = seek_block(content_block_posi << 6);

	string str = "";
	int next_block;
	do {
		//int *bits_map = content_block.get_bits_map();
		int *content = content_block.get_cont();
		char *p;
		p = (char*)content;
		string ss(p);
		str += ss;
		next_block = content_block.get_next();
		if (next_block)content_block = seek_block(next_block);
	} while (next_block);
	cout << str << endl;
}

void more_v_write(int block_num)
{
	string str = "";
	int str_size;
	while (true)
	{
		char c;
		cin.get(c);
		str += c;
		str_size = str.size();
		if (str_size > 4 && str[str_size - 4] == '#'&&str[str_size - 3] == 'e'&&str[str_size - 2] == 'o'&&str[str_size - 1] == 'f')
			break;
	}
	str = str.substr(0, str_size - 4);

	int length = str.size();
	char ch[244];
	int index = 0;
	while (length > 0)
	{
		int copy_len = (length > 243) ? 243 : length;
		length -= copy_len;
		for (size_t i = 0; i < copy_len; i++)
			ch[i] = str[index++];
		ch[copy_len] = 0;
		Block b;
		b.add_string(ch);
		int bits_len = (copy_len % 4) ? copy_len / 4 + 1 : copy_len / 4;
		b.set_bits_map(2, 2 + bits_len);
		if (length > 0) {
			int next_block_num = super_block.find_new_block();
			b.set_next(next_block_num << 6);
			super_block.use_block(next_block_num);
			write_block(b, block_num << 6);
			write_big_block(super_block);
			block_num = next_block_num;
		}
		else
		{
			write_block(b, block_num << 6);
			write_big_block(super_block);
		}
	}
}

void del_file_content(int block_num)//指针first_block没有置0
{
	Block b = seek_block(block_num << 6);
	vector<int> content_block;
	content_block.push_back(block_num << 6);
	while (b.get_next())
	{
		content_block.push_back(b.get_next());
		b = seek_block(b.get_next());
	}
	for (size_t i = 0; i < content_block.size(); i++)
	{
		Block empty_block;
		write_block(empty_block, content_block[i]);
		super_block.free_block(content_block[i] >> 6);
	}
	write_big_block(super_block);
}

void more_v(const char file_name[])
{
	int mask = 0b111111;
	int file_posi = find_file(file_name);
	if (!file_posi) {
		cout << "文件不存在" << endl;
		return;
	}
	File file = seek_file(file_posi);
	//添加内容
	if (file.get_block())//内容不空 删掉原来的内容
	{
		//先把原来的删了(指针first_block没置0)
		int block_num = file.get_block();
		del_file_content(block_num);
		//再写新的
		super_block.use_block(block_num);
		write_big_block(super_block);
		more_v_write(block_num);

	}
	else//内容为空
	{
		//为文件添加块，从console获取字符串
		int block_num = super_block.find_new_block();
		file.set_block(block_num);
		Block file_block = seek_block((file_posi >> 6) << 6);
		file_block.add_file(file, file_posi&mask);
		write_block(file_block, (file_posi >> 6) << 6);
		super_block.use_block(block_num);
		write_big_block(super_block);
		more_v_write(block_num);
	}
	char ch;
	while ((ch = getchar()) != '\n' && ch != EOF);//清空缓冲区
}

int get_folder_posi(Folder folder)
{
	Pointer f_folder_posi = seek_pointer(folder.get_parent_floder().get_next());
	Folder f_folder = seek_folder(f_folder_posi.get_addr());

	Pointer folders_ptr = f_folder.get_child_floder();
	int folders_addr = 0;
	while (folders_ptr.get_next() != 0)
	{
		folders_addr = folders_ptr.get_addr();
		if (!folders_addr)
		{
			folders_ptr = seek_pointer(folders_ptr.get_next());
			continue;
		}
		Folder the_folder = seek_folder(folders_addr);
		if (strcmp(the_folder.get_name(), folder.get_name()) == 0)
			return folders_addr;
		folders_ptr = seek_pointer(folders_ptr.get_next());
	}
	return 0;
}

bool del_file(const char file_name[])
{
	int mask = 0b111111;
	int file_posi = find_file(file_name);
	if (!file_posi)return false;

	//找出文件,所在文件夹,所在块和文件内容
	File file = seek_file(file_posi);
	Block file_block = seek_block((file_posi >> 6) << 6);
	Folder folder = seek_folder(file.get_folder());
	int content_block_posi = file.get_block();
	//删除文件内容
	if (content_block_posi) del_file_content(content_block_posi);
	//删除文件
	file_block.free_file(file_posi&mask);
	write_block(file_block, (file_posi >> 6) << 6);
	//删除文件夹中的指针
	Pointer file_ptr = folder.get_files();
	File file_in_folder;
	vector<int> addrs;
	vector<int> nexts;
	int index = 0;
	while (file_ptr.get_next() != 0)
	{
		addrs.push_back(file_ptr.get_addr());
		nexts.push_back(file_ptr.get_next());
		file_ptr = seek_pointer(file_ptr.get_next());
	}
	for (size_t i = 1; i < addrs.size(); i++)
	{
		file_in_folder = seek_file(addrs[i]);
		if (strcmp(file_in_folder.get_name(), file_name) == 0) {
			index = i;
			break;
		}
	}
	if (index == 0) return false;
	else {
		Block ptr_block = seek_block((nexts[index - 1] >> 6) << 6);
		ptr_block.free_pointer(nexts[index - 1] & mask);
		write_block(ptr_block, (nexts[index - 1] >> 6) << 6);
	}
	if (index == 1) {
		now_folder.set_files_next(nexts[1]);
		folder_block = seek_block((now_folder_posi >> 6) << 6);
		folder_block.add_folder(now_folder, now_folder_posi&mask);
		write_block(folder_block, (now_folder_posi >> 6) << 6);
		return true;
	}
	else {
		Pointer pre_ptr = seek_pointer(nexts[index - 2]);
		Block pre_ptr_block = seek_block((nexts[index - 2] >> 6) << 6);
		pre_ptr.set_next(nexts[index]);
		pre_ptr_block.add_pointer(pre_ptr, nexts[index - 2] & mask);
		write_block(pre_ptr_block, (nexts[index - 2] >> 6) << 6);
		return true;
	}
}

bool folder_is_empty(Folder folder)
{
	Pointer sub_folder_ptr = seek_pointer(folder.get_child_floder().get_next());
	Pointer sub_file_ptr = seek_pointer(folder.get_files().get_next());
	if (sub_file_ptr.get_addr() || sub_folder_ptr.get_addr())return false;
	return true;
}

bool rmdir(const char folder_name[])
{
	int mask = 0b111111;
	//找子文件夹，可以考虑单列一个函数
	int addr = find_folder(folder_name);
	if (!addr) {
		cout << "文件夹不存在" << endl;
		return false;
	}
	Folder folder = seek_folder(addr);

	if (!folder_is_empty(folder)) {
		cout << "文件夹不为空" << endl;
		return false;
	}
	//如果文件夹为空，删除文件夹
	Block the_folder_block = seek_block((addr >> 6) << 6);
	the_folder_block.free_dir(addr&mask);
	write_block(the_folder_block, (addr >> 6) << 6);

	Pointer child_folder_ptr = now_folder.get_child_floder();
	Folder folder_in_folder;
	vector<int> addrs;
	vector<int> nexts;
	int index = 0;
	while (child_folder_ptr.get_next())
	{
		addrs.push_back(child_folder_ptr.get_addr());
		nexts.push_back(child_folder_ptr.get_next());
		child_folder_ptr = seek_pointer(child_folder_ptr.get_next());
	}
	for (size_t i = 1; i < addrs.size(); i++)
	{
		folder_in_folder = seek_folder(addrs[i]);
		if (strcmp(folder_in_folder.get_name(), folder_name) == 0) {
			index = i;
			break;
		}
	}
	if (index == 0)return false;
	else {
		Block ptr_block = seek_block((nexts[index - 1] >> 6) << 6);
		ptr_block.free_pointer(nexts[index - 1] & mask);
		write_block(ptr_block, (nexts[index - 1] >> 6) << 6);
	}
	if (index == 1)
	{
		//这里如果是删除的是前文件夹下的子文件夹就可以直接用now_folder和folder_block
		//昨天的del_file中的if(index == 1)也是同理，但是后者用了更复杂的方法
		now_folder.set_child_next(nexts[1]);
		folder_block.add_folder(now_folder, now_folder_posi&mask);
		write_block(folder_block, (now_folder_posi >> 6) << 6);
		return true;
	}
	else
	{
		Pointer pre_ptr = seek_pointer(nexts[index - 2]);
		Block pre_ptr_block = seek_block((nexts[index - 2] >> 6) << 6);
		pre_ptr.set_next(nexts[index]);
		pre_ptr_block.add_pointer(pre_ptr, nexts[index - 2] & mask);
		write_block(pre_ptr_block, (nexts[index - 2] >> 6) << 6);
		return true;
	}
}

void rm_about_folder(Folder folder, vector<int>&nexts, vector<int>&folder_addr, vector<int>&file_addr)
{
	Pointer child_folder_ptr = folder.get_child_floder();
	int addr;
	while (child_folder_ptr.get_next() != 0)
	{
		addr = child_folder_ptr.get_addr();
		if (!addr) {
			child_folder_ptr = seek_pointer(child_folder_ptr.get_next());
			continue;
		}
		Folder child_folder = seek_folder(addr);
		Pointer pointers = child_folder.get_child_floder();
		while (pointers.get_next())
		{
			folder_addr.push_back(pointers.get_addr());
			nexts.push_back(pointers.get_next());
			pointers = seek_pointer(pointers.get_next());
		}
		pointers = child_folder.get_files();
		while (pointers.get_next())
		{
			file_addr.push_back(pointers.get_addr());//第0个是0，删除时要判断
			nexts.push_back(pointers.get_next());
			pointers = seek_pointer(pointers.get_next());
		}
		nexts.push_back(child_folder.get_parent_floder().get_next());
		rm_about_folder(child_folder, nexts, folder_addr, file_addr);
		child_folder_ptr = seek_pointer(child_folder_ptr.get_next());
	}
	Pointer files_ptr = folder.get_files();
	while (files_ptr.get_next() != 0)
	{
		addr = files_ptr.get_addr();
		if (!addr) {
			files_ptr = seek_pointer(files_ptr.get_next());
			continue;
		}
		File file = seek_file(addr);
		vector<int>block_addr;
		block_addr.push_back(file.get_block() << 6);
		Block b = seek_block(file.get_block() << 6);
		while (b.get_next())
		{
			block_addr.push_back(b.get_next());
			b = seek_block(b.get_next());
		}
		Block empty_block;
		for (size_t i = 0; i < block_addr.size(); i++)
		{
			write_block(empty_block, block_addr[i]);
			super_block.free_block(block_addr[i] >> 6);
		}
		files_ptr = seek_pointer(files_ptr.get_next());
	}
}

bool rmdir_rf(const char folder_name[])
{
	int mask = 0b111111;
	int folder_posi = find_folder(folder_name);
	if (!folder_posi)return false;
	Folder folder = seek_folder(folder_posi);
	vector<int>next_s;
	vector<int>folder_addr;
	vector<int>file_addr;
	rm_about_folder(folder, next_s, folder_addr, file_addr);
	for (size_t i = 0; i < next_s.size(); i++)
	{
		Block b = seek_block((next_s[i] >> 6) << 6);
		b.free_pointer(next_s[i] & mask);
		write_block(b, (next_s[i] >> 6) << 6);
	}
	for (size_t i = 0; i < folder_addr.size(); i++)
	{
		if (folder_addr[i]) {
			Block b = seek_block((folder_addr[i] >> 6) << 6);
			b.free_dir(folder_addr[i] & mask);
			write_block(b, (folder_addr[i] >> 6) << 6);
		}
	}
	for (size_t i = 0; i < file_addr.size(); i++)
	{
		if (file_addr[i])
		{
			Block b = seek_block((file_addr[i] >> 6) << 6);
			b.free_file(folder_addr[i] & mask);
			write_block(b, (folder_addr[i] >> 6) << 6);
		}
	}

	Block the_folder_block = seek_block((folder_posi >> 6) << 6);
	the_folder_block.free_dir(folder_posi&mask);
	write_block(the_folder_block, (folder_posi >> 6) << 6);

	Pointer child_folder_ptr = now_folder.get_child_floder();
	Folder folder_in_folder;
	vector<int> addrs;
	vector<int> nexts;
	int index = 0;
	while (child_folder_ptr.get_next())
	{
		addrs.push_back(child_folder_ptr.get_addr());
		nexts.push_back(child_folder_ptr.get_next());
		child_folder_ptr = seek_pointer(child_folder_ptr.get_next());
	}
	for (size_t i = 1; i < addrs.size(); i++)
	{
		folder_in_folder = seek_folder(addrs[i]);
		if (strcmp(folder_in_folder.get_name(), folder_name) == 0) {
			index = i;
			break;
		}
	}
	if (index == 0)return false;
	else {
		Block ptr_block = seek_block((nexts[index - 1] >> 6) << 6);
		ptr_block.free_pointer(nexts[index - 1] & mask);
		write_block(ptr_block, (nexts[index - 1] >> 6) << 6);
	}
	if (index == 1)
	{
		//这里如果是删除的是前文件夹下的子文件夹就可以直接用now_folder和folder_block
		//昨天的del_file中的if(index == 1)也是同理，但是后者用了更复杂的方法
		now_folder.set_child_next(nexts[1]);
		folder_block.add_folder(now_folder, now_folder_posi&mask);
		write_block(folder_block, (now_folder_posi >> 6) << 6);
		return true;
	}
	else
	{
		Pointer pre_ptr = seek_pointer(nexts[index - 2]);
		Block pre_ptr_block = seek_block((nexts[index - 2] >> 6) << 6);
		pre_ptr.set_next(nexts[index]);
		pre_ptr_block.add_pointer(pre_ptr, nexts[index - 2] & mask);
		write_block(pre_ptr_block, (nexts[index - 2] >> 6) << 6);
		return true;
	}
}

int goto_folder(const char folder_way[], Folder &folder)
{
	string folder_str(folder_way);
	string str = "";
	vector<string> vector_dirs;
	int posi = 0;
	for (size_t i = 0; i < folder_str.size(); i++)
	{
		if (folder_str[i] == '/') {
			vector_dirs.push_back(str);
			str = "";
		}
		else str += folder_str[i];
	}
	vector_dirs.push_back(str);
	if (vector_dirs[0] == "") //绝对路径
	{
		folder = seek_folder(130);
		posi = 130;
	}
	else folder = seek_folder(now_folder_posi);//相对路径

	for (size_t i = 0; i < vector_dirs.size(); i++)
	{
		if (vector_dirs[i] == "..")
		{
			Pointer pointer = seek_pointer(folder.get_parent_floder().get_next());
			folder = seek_folder(pointer.get_addr());
			posi = pointer.get_addr();
		}
		else if (vector_dirs[i] == "." || vector_dirs[i] == "")continue;
		else {
			posi = find_any_folder(folder, vector_dirs[i]);
			if (posi == 0)return 0;
			folder = seek_folder(posi);
		}
	}
	return posi;
}

bool move_f(const char file_name[], const char folder_name[])
{
	int mask = 0b111111;

	//去掉子文件指针
	Pointer file_ptr = now_folder.get_files();
	File file_in_folder;
	vector<int> addrs;
	vector<int> nexts;
	int index = 0;
	while (file_ptr.get_next() != 0)
	{
		addrs.push_back(file_ptr.get_addr());
		nexts.push_back(file_ptr.get_next());
		file_ptr = seek_pointer(file_ptr.get_next());
	}
	for (size_t i = 1; i < addrs.size(); i++)
	{
		file_in_folder = seek_file(addrs[i]);
		if (strcmp(file_in_folder.get_name(), file_name) == 0) {
			index = i;
			break;
		}
	}
	if (index == 0) return false;
	else {
		Block ptr_block = seek_block((nexts[index - 1] >> 6) << 6);
		ptr_block.free_pointer(nexts[index - 1] & mask);
		write_block(ptr_block, (nexts[index - 1] >> 6) << 6);
	}
	if (index == 1) {
		now_folder.set_files_next(nexts[1]);
		//如不需要可以替换为now_folder_posi(如果删除的是当前文件夹下的文件)
		//int folder_posi = get_folder_posi(folder);
		int folder_posi = now_folder_posi;
		if (folder_posi == 0)return false;
		Block f_block = seek_block((folder_posi >> 6) << 6);
		f_block.add_folder(now_folder, folder_posi&mask);
		write_block(f_block, (folder_posi >> 6) << 6);
	}
	else {
		Pointer pre_ptr = seek_pointer(nexts[index - 2]);
		Block pre_ptr_block = seek_block((nexts[index - 2] >> 6) << 6);
		pre_ptr.set_next(nexts[index]);
		pre_ptr_block.add_pointer(pre_ptr, nexts[index - 2] & mask);
		write_block(pre_ptr_block, (nexts[index - 2] >> 6) << 6);
	}

	//取出子文件指针
	int addr = addrs[index];

	//添加到文件夹folder
	Folder folder;
	int folder_addr = goto_folder(folder_name, folder);
	if (!folder_addr)return false;
	int pointer_next = folder.get_files().get_next();

	file_in_folder.set_folder(folder_addr);
	Block fif_block = seek_block((addr >> 6) << 6);
	fif_block.add_file(file_in_folder, addr&mask);
	write_block(fif_block, (addr >> 6) << 6);

	Pointer pointer;
	int next_addr = get_pointer(pointer, addr, pointer_next);
	folder.set_files_next(next_addr);
	Block finded_fdr_block = seek_block((folder_addr >> 6) << 6);
	finded_fdr_block.add_folder(folder, folder_addr&mask);
	write_block(finded_fdr_block, (folder_addr >> 6) << 6);
	return true;
}

bool move_folder(const char folder_name[], const char folder_way[])
{
	int mask = 0b111111;

	//去掉子文件夹指针
	Pointer folder_ptr = now_folder.get_child_floder();
	Folder folder_in_folder;
	vector<int> addrs;
	vector<int> nexts;
	int index = 0;
	while (folder_ptr.get_next() != 0)
	{
		addrs.push_back(folder_ptr.get_addr());
		nexts.push_back(folder_ptr.get_next());
		folder_ptr = seek_pointer(folder_ptr.get_next());
	}
	for (size_t i = 1; i < addrs.size(); i++)
	{
		folder_in_folder = seek_folder(addrs[i]);
		if (strcmp(folder_in_folder.get_name(), folder_name) == 0) {
			index = i;
			break;
		}
	}
	if (index == 0) return false;
	else {
		Block ptr_block = seek_block((nexts[index - 1] >> 6) << 6);
		ptr_block.free_pointer(nexts[index - 1] & mask);
		write_block(ptr_block, (nexts[index - 1] >> 6) << 6);
	}
	if (index == 1) {
		now_folder.set_child_next(nexts[1]);
		//如不需要可以替换为now_folder_posi(如果删除的是当前文件夹下的文件)
		//int folder_posi = get_folder_posi(folder);
		int folder_posi = now_folder_posi;
		if (folder_posi == 0)return false;
		Block f_block = seek_block((folder_posi >> 6) << 6);
		f_block.add_folder(now_folder, folder_posi&mask);
		write_block(f_block, (folder_posi >> 6) << 6);
	}
	else {
		Pointer pre_ptr = seek_pointer(nexts[index - 2]);
		Block pre_ptr_block = seek_block((nexts[index - 2] >> 6) << 6);
		pre_ptr.set_next(nexts[index]);
		pre_ptr_block.add_pointer(pre_ptr, nexts[index - 2] & mask);
		write_block(pre_ptr_block, (nexts[index - 2] >> 6) << 6);
	}

	//取出子文件夹指针
	int addr = addrs[index];
	//添加到文件夹folder
	Folder folder;
	int folder_addr = goto_folder(folder_way, folder);
	if (!folder_addr)return false;
	int pointer_next = folder.get_child_floder().get_next();

	Pointer p_pointer;
	int p = get_pointer(p_pointer, folder_addr, 0);
	folder_in_folder.set_parent_next(p);
	Block fif_block = seek_block((addr >> 6) << 6);
	fif_block.add_folder(folder_in_folder, addr&mask);
	write_block(fif_block, (addr >> 6) << 6);

	Pointer pointer;
	int next_addr = get_pointer(pointer, addr, pointer_next);
	folder.set_child_next(next_addr);
	Block finded_fdr_block = seek_block((folder_addr >> 6) << 6);
	finded_fdr_block.add_folder(folder, folder_addr&mask);
	write_block(finded_fdr_block, (folder_addr >> 6) << 6);
	return true;
}

bool copy_f(const char file_name[], const char folder_name[])
{
	int mask = 0b111111;
	int file_posi = find_file(file_name);
	if (!file_posi)return false;

	//存文件的所有块到blocks
	File file = seek_file(file_posi);
	int first_posi = 0;
	if (file.get_block()) {
		Block b = seek_block(file.get_block() << 6);
		vector<int> blocks;
		blocks.push_back(file.get_block() << 6);
		while (b.get_next())
		{
			blocks.push_back(b.get_next());
			b = seek_block(b.get_next());
		}

		first_posi = super_block.find_new_block();
		super_block.use_block(first_posi);
		int now_posi = first_posi;
		for (size_t i = 0; i < blocks.size() - 1; i++)
		{
			int next_posi = super_block.find_new_block();
			super_block.use_block(next_posi);
			Block blk = seek_block(blocks[i]);
			blk.set_next(next_posi << 6);
			write_block(blk, now_posi << 6);
			now_posi = next_posi;
		}
		Block blk = seek_block(blocks[blocks.size() - 1]);
		write_block(blk, now_posi << 6);
		write_big_block(super_block);
	}

	Folder folder;
	int folder_addr = goto_folder(folder_name, folder);
	if (!folder_addr)return false;

	File copyed_file(file.get_name(), first_posi, folder_addr, 0);
	int file_addr = find4file();
	Block file_block = seek_block((file_addr >> 6) << 6);
	file_block.add_file(copyed_file, file_addr&mask);
	write_block(file_block, (file_addr >> 6) << 6);
	super_block.use_block((file_addr >> 6));
	write_big_block(super_block);

	Pointer pointer;
	int ptr_posi = get_pointer(pointer, file_addr, folder.get_files().get_next());
	folder.set_files_next(ptr_posi);
	Block fd_block = seek_block((folder_addr >> 6) << 6);
	fd_block.add_folder(folder, folder_addr&mask);
	write_block(fd_block, (folder_addr >> 6) << 6);
	return true;
}

bool copy_creat(Folder folder)
{
	int mask = 0b111111;
	Pointer child_folder_ptr = folder.get_child_floder();
	int addr;
	while (child_folder_ptr.get_next() != 0)
	{
		addr = child_folder_ptr.get_addr();
		if (!addr) {
			child_folder_ptr = seek_pointer(child_folder_ptr.get_next());
			continue;
		}
		Folder child_folder = seek_folder(addr);

		mkdir(child_folder.get_name());
		cd_in(child_folder.get_name());
		cout << "|-" << child_folder.get_name() << endl;
		copy_creat(child_folder);
		cd_out();
		child_folder_ptr = seek_pointer(child_folder_ptr.get_next());
	}
	Pointer files_ptr = folder.get_files();
	while (files_ptr.get_next() != 0)
	{
		addr = files_ptr.get_addr();
		if (!addr) {
			files_ptr = seek_pointer(files_ptr.get_next());
			continue;
		}
		File file = seek_file(addr);

		//复制文件
		Block b = seek_block(file.get_block() << 6);
		vector<int> blocks;
		blocks.push_back(file.get_block() << 6);
		while (b.get_next())
		{
			blocks.push_back(b.get_next());
			b = seek_block(b.get_next());
		}

		int first_posi = super_block.find_new_block();
		super_block.use_block(first_posi);
		int now_posi = first_posi;
		for (size_t i = 0; i < blocks.size() - 1; i++)
		{
			int next_posi = super_block.find_new_block();
			super_block.use_block(next_posi);
			Block blk = seek_block(blocks[i]);
			blk.set_next(next_posi << 6);
			write_block(blk, now_posi << 6);
			now_posi = next_posi;
		}
		Block blk = seek_block(blocks[blocks.size() - 1]);
		write_block(blk, now_posi << 6);
		write_big_block(super_block);

		File copyed_file(file.get_name(), first_posi, now_folder_posi, 0);
		int file_addr = find4file();
		Block file_block = seek_block((file_addr >> 6) << 6);
		file_block.add_file(copyed_file, file_addr&mask);
		write_block(file_block, (file_addr >> 6) << 6);
		super_block.use_block((file_addr >> 6));
		write_big_block(super_block);

		Pointer pointer;
		int ptr_posi = get_pointer(pointer, file_addr, now_folder.get_files().get_next());
		now_folder.set_files_next(ptr_posi);
		Block fd_block = seek_block((now_folder_posi >> 6) << 6);
		fd_block.add_folder(now_folder, now_folder_posi&mask);
		write_block(fd_block, (now_folder_posi >> 6) << 6);

		files_ptr = seek_pointer(files_ptr.get_next());
	}
	return false;
}

bool copy_folder(const char folder_name[], const char folder_way[])
{
	int folder_posi = find_folder(folder_name);
	if (!folder_posi)return 0;
	Folder folder = seek_folder(folder_posi);

	int store_posi = now_folder_posi;
	now_folder_posi = goto_folder(folder_way, now_folder);
	mkdir(folder_name);
	cd_in(folder_name);

	copy_creat(folder);

	now_folder_posi = store_posi;
	now_folder = seek_folder(now_folder_posi);
	return true;
}

void import_file(const char file_name[], string local_way)
{
	touch(file_name);
	int mask = 0b111111;
	int file_posi = find_file(file_name);
	if (!file_posi) {
		cout << "文件不存在" << endl;
		return;
	}
	File file = seek_file(file_posi);

	int block_num = super_block.find_new_block();
	file.set_block(block_num);
	Block file_block = seek_block((file_posi >> 6) << 6);
	file_block.add_file(file, file_posi&mask);
	write_block(file_block, (file_posi >> 6) << 6);
	super_block.use_block(block_num);
	write_big_block(super_block);
	
	ifstream in(local_way);
	string str;
	char c;
	while (!in.eof())
	{
		in >> c;
		str += c;
	}

	int length = str.size();
	char ch[244];
	int index = 0;
	while (length > 0)
	{
		int copy_len = (length > 243) ? 243 : length;
		length -= copy_len;
		for (size_t i = 0; i < copy_len; i++)
			ch[i] = str[index++];
		ch[copy_len] = 0;
		Block b;
		b.add_string(ch);
		int bits_len = (copy_len % 4) ? copy_len / 4 + 1 : copy_len / 4;
		b.set_bits_map(2, 2 + bits_len);
		if (length > 0) {
			int next_block_num = super_block.find_new_block();
			b.set_next(next_block_num << 6);
			super_block.use_block(next_block_num);
			write_block(b, block_num << 6);
			write_big_block(super_block);
			block_num = next_block_num;
		}
		else
		{
			write_block(b, block_num << 6);
			write_big_block(super_block);
		}
	}
}

void export_file(const char file_name[], string local_way)
{
	if (!find_file(file_name)) {
		cout << "文件不存在" << endl;
		return;
	}
	File file = seek_file(find_file(file_name));
	int content_block_posi = file.get_block();
	Block content_block = seek_block(content_block_posi << 6);

	string str = "";
	int next_block;
	do {
		//int *bits_map = content_block.get_bits_map();
		int *content = content_block.get_cont();
		char *p;
		p = (char*)content;
		string ss(p);
		str += ss;
		next_block = content_block.get_next();
		if (next_block)content_block = seek_block(next_block);
	} while (next_block);

	ofstream out(local_way);
	out << str << endl;
}
