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
		if (((cur = cur->succ)->runTime) <= (min->runTime))//��cur>=max
			min = cur;
	return min;//��������ʱ����ڵ�λ�� 
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
		//tail = tail->pred;//ÿ�ΰ����ķŵ�tailǰ�棬tailǰ��
		n--;
	}
}

void ProcessQueue::print()
{
	Process *p = header->succ;
	cout << "��������\t״̬\t���ȼ�\t��������ʱ��" <<endl;
	while (p->succ)
	{
		cout << p->name << "\t" << p->status << "\t" << p->priority << "\t" << p->runTime << endl;
		p = p->succ;
	}
	cout << "����������������������������������������������������" << endl;
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
	cout << "�������ʱ����ȷ���" << endl;
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
	cout << "��������:" << endl;
	readyQueue.print();
	cout << "�ȴ�����:" << endl;
	waitQueue.print();
	cout << "��ɶ���:" << endl;
	finishQueue.print();
}

void executeProcess()
{
	if (getRandomNum(0, 1) && !waitQueue.isEmpty()) {
		Process *eq = waitQueue.dequeue();
		readyQueue.enqueue(*eq);
		cout << eq->name << "�Ѽ����������" << endl;
		readyQueue.selectionSort();
	}
	else
		cout << "û�н��̼����������" << endl;
	Process *p = readyQueue.dequeue();
	int time = getRandomNum(1, 20);
	cout << p->name << "����ʱ�䣺" << p->runTime << "��ʱ��Ƭ��СΪ" << time << endl;
	if (p->runTime - time <= 0)
	{
		finishQueue.enqueue(p->name, p->status, p->priority, 0);
		cout << p->name << "�Ѽ�����ɶ���" << endl;
	}
	else
	{
		if (getRandomNum(0, 1))
		{
			waitQueue.enqueue(p->name, p->status, p->priority, p->runTime - time);
			waitQueue.selectionSort();
			cout << p->name << "�Ѽ���ȴ����У�����ʱ��" << p->runTime - time << endl;
		}
		else
		{
			readyQueue.enqueue(p->name, p->status, p->priority, p->runTime - time);
			readyQueue.selectionSort();
			cout << p->name << "�Ѽ���������У�����ʱ��" << p->runTime - time << endl;
		}
	}
	cout << "����������������������������������������������������" << endl;
}

int main()
{
	init();
	while(!readyQueue.isEmpty())
		executeProcess();
	cout << "��������:" << endl;
	readyQueue.print();
	cout << "�ȴ�����:" << endl;
	waitQueue.print();
	cout << "��ɶ���:" << endl;
	finishQueue.print();
	return 0;
}
