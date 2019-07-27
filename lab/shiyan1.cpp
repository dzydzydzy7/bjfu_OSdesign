#include <iostream>
#include <string>
using namespace std;

struct Process
{
	string name;
	int status;
	int priority;
	int runTime;
	Process *pred;
	Process *succ;
	Process() {}
	Process(string nm, int st, int pr, int rt, Process *p = NULL, Process *s = NULL):\
		name(nm), status(st), priority(pr), runTime(rt), pred(p), succ(s){}
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
	return enqueue(p.name, p.status, p.priority, p.runTime);
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
		enqueue(p->name, p->status, p->priority, p->runTime);
		//tail = tail->pred;//每次把最大的放到tail前面，tail前移
		n--;
	}
}

void ProcessQueue::print()
{
	Process *p = header->succ;
	cout << "进程名称\t状态\t优先级\t还需运行时间" <<endl;
	while (p->succ)
	{
		cout << p->name << "\t" << p->status << "\t" << p->priority << "\t" << p->runTime << endl;
		p = p->succ;
	}
	cout << "——————————————————————————" << endl;
}

bool ProcessQueue::isEmpty()
{
	return header->succ == tailer;
}

ProcessQueue readyQueue;
ProcessQueue waitQueue;
ProcessQueue finishQueue;


int getRandomNum(int start, int end)
{
	return (rand() % (end - start + 1)) + start;
}

void init()
{
	cout << "最短运行时间调度法：" << endl;
	for (int i = 0; i < 10; i++)
	{
		string str = "process";
		char c = 48 + i;
		Process p(str + c, getRandomNum(0, 1), getRandomNum(1, 10), getRandomNum(10, 50));
		if (!p.status)readyQueue.enqueue(p);
		else waitQueue.enqueue(p);
	}
	readyQueue.selectionSort();
	waitQueue.selectionSort();
	cout << "就绪队列:" << endl;
	readyQueue.print();
	cout << "等待队列:" << endl;
	waitQueue.print();
	cout << "完成队列:" << endl;
	finishQueue.print();
}

void executeProcess()
{
	if (getRandomNum(0, 1) && !waitQueue.isEmpty()) {
		Process *eq = waitQueue.dequeue();
		readyQueue.enqueue(*eq);
		cout << eq->name << "已加入就绪队列" << endl;
		readyQueue.selectionSort();
	}
	else
		cout << "没有进程加入就绪队列" << endl;
	Process *p = readyQueue.dequeue();
	int time = getRandomNum(1, 20);
	cout << p->name << "还需时间：" << p->runTime << "，时间片大小为" << time << endl;
	if (p->runTime - time <= 0)
	{
		finishQueue.enqueue(p->name, p->status, p->priority, 0);
		cout << p->name << "已加入完成队列" << endl;
	}
	else
	{
		if (getRandomNum(0, 1))
		{
			waitQueue.enqueue(p->name, p->status, p->priority, p->runTime - time);
			waitQueue.selectionSort();
			cout << p->name << "已加入等待队列，还需时间" << p->runTime - time << endl;
		}
		else
		{
			readyQueue.enqueue(p->name, p->status, p->priority, p->runTime - time);
			readyQueue.selectionSort();
			cout << p->name << "已加入就绪队列，还需时间" << p->runTime - time << endl;
		}
	}
	cout << "——————————————————————————" << endl;
}

int main()
{
	init();
	while(!readyQueue.isEmpty())
		executeProcess();
	cout << "就绪队列:" << endl;
	readyQueue.print();
	cout << "等待队列:" << endl;
	waitQueue.print();
	cout << "完成队列:" << endl;
	finishQueue.print();
	return 0;
}
