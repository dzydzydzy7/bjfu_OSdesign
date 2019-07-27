#include <iostream>
#include <string>
using namespace std;

const int processNum = 10;//进程数量 = 生产者 + 消费者
const int bufferSize = 1;	//buffer数量
int buffer[bufferSize];

struct Process
{
	string name;
	int status;	//0 wait 1 ready
	int type;	//0 producer 1 custumer
	int runTime;
	Process *pred;
	Process *succ;
	Process() {}
	Process(string nm, int st, int t, int rt, Process *p = NULL, Process *s = NULL):\
		name(nm), status(st), type(t), runTime(rt), pred(p), succ(s){}
};

class ProcessQueue
{
private:
	int _size;
	Process *header;
	Process *tailer;
public:
	ProcessQueue();
	~ProcessQueue() {};
	Process* enqueue(string n, int s, int p, int r);
	Process* enqueue(Process p);
	Process* dequeue();
	Process* selectMin(Process* p, int n);
	void selectionSort();
	void print();
	bool isEmpty();
};

ProcessQueue::ProcessQueue()
{
	header = new Process;
	tailer = new Process;
	header->succ = tailer;
	header->pred = NULL;
	tailer->pred = header;
	tailer->succ = NULL;
	_size = 0;
}

Process * ProcessQueue::enqueue(string n, int s, int p, int r)
{
	_size++;
	Process *x = new Process(n, s, p, r);
	Process *y = tailer->pred;
	y->succ = x;
	x->pred = y;
	x->succ = tailer;
	tailer->pred = x;
	return x;
}

Process * ProcessQueue::enqueue(Process p)
{
	return enqueue(p.name, p.status, p.type, p.runTime);
}

Process * ProcessQueue::dequeue()
{
	Process *p = header->succ;
	p->pred->succ = p->succ;
	p->succ->pred = p->pred;
	_size--;
	return p;
}

Process * ProcessQueue::selectMin(Process* p, int n)
{
	Process* min = p;
	for (Process* cur = p; 1 < n; n--)
		if (((cur = cur->succ)->runTime) <= (min->runTime))//若cur>=max
			min = cur;
	return min;//返回运行时间最长节点位置 
}

void ProcessQueue::selectionSort()
{
	Process* head = header;
	Process* tail = tailer;
	int n = _size;
	while (0 < n)
	{
		//insertBefore(tail, remove(selectMax(head->succ, n)));
		Process* p = selectMin(head->succ, n);
		p->pred->succ = p->succ;
		p->succ->pred = p->pred;
		_size--;
		enqueue(p->name, p->status, p->type, p->runTime);
		//tail = tail->pred;//每次把最大的放到tail前面，tail前移
		n--;
	}
}

void ProcessQueue::print()
{
	Process *p = header->succ;
	cout << "进程名称\t状态\t类型\t还需运行时间" <<endl;
	while (p->succ)
	{
		cout << p->name << "\t" << p->status << "\t" << p->type << "\t" << p->runTime << endl;
		p = p->succ;
	}
	cout << "――――――――――――――――――――――――――" << endl;
}

bool ProcessQueue::isEmpty()
{
	return header->succ == tailer;
}

ProcessQueue readyQueue;
ProcessQueue producerWaitQueue;
ProcessQueue custumerWaitQueue;

void producer(Process p)
{
	bool isGetBuffer = false;
	for(int i = 0; i < bufferSize; i++)
	{
		if(buffer[i] == 0)
		{
			buffer[i] = 1;
			isGetBuffer = true;
			readyQueue.enqueue(p);
			break;
		}
	}
	if(isGetBuffer == false)
	{
		producerWaitQueue.enqueue(p);
	}
}

void custumer(Process p)
{
	bool isUseBuffer = false;
	for(int i = 0; i < bufferSize; i++)
	{
		if(buffer[i] == 1)
		{
			buffer[i] = 0;
			isUseBuffer = true;
			readyQueue.enqueue(p);
			break;
		}
	}
	if(isUseBuffer == false)
	{
		custumerWaitQueue.enqueue(p);
	}
}

int getRandomNum(int start, int end)
{
	return (rand() % (end - start + 1)) + start;
}

void init()
{
	int j = 0, k = 0;
	for (int i = 0; i < processNum; i++)
	{	
		if(getRandomNum(0, 1)){
			string str = "producer";
			char c = 48 + k;
			k++;
			Process p(str + c, 0, 0, getRandomNum(10, 50));
			producerWaitQueue.enqueue(p);
		}
		else
		{
			string str = "custumer";
			char c = 48 + j;
			j++;
			Process p(str + c, 0, 1, getRandomNum(10, 50));
			custumerWaitQueue.enqueue(p);
			
		}
	}
	printf("producer wait queue:\n");
	producerWaitQueue.print();
	printf("custumer wait queue:\n");
	custumerWaitQueue.print();
}

int main()
{
	init();
	while(!producerWaitQueue.isEmpty() || !custumerWaitQueue.isEmpty())
	{
		if(!producerWaitQueue.isEmpty())
			producer(*producerWaitQueue.dequeue());
		if(!custumerWaitQueue.isEmpty())
			custumer(*custumerWaitQueue.dequeue());
		printf("producer wait queue:\n");
		producerWaitQueue.print();
		printf("custumer wait queue:\n");
		custumerWaitQueue.print();
		printf("ready queue:\n");
		readyQueue.print();
		bool flag = true; //全都一样
		int last = buffer[0];
		for(int i = 0; i < bufferSize; i++)
		{
			if(buffer[i] != last) flag = false;
		}
		if(custumerWaitQueue.isEmpty() && buffer[0] == 1 && flag) break;
		if(producerWaitQueue.isEmpty() && buffer[0] == 0 && flag) break;
	}
	return 0;
}