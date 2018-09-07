//HW4

#include <time.h>
#include<fstream>
#include <iostream>
#include<string>
#include <list>
#include<queue>
using namespace std;

#define PAGENUM 10
#define TESTSIZE 100

#define MYTYPE int

class LRU {
public:
	LRU() {
		memset(frames, 0, sizeof(frames));
	}

	~LRU() {

	}

	int getPageFault() {
		return pageFault;
	}

	void print(string s) {
		cout << s << " current frames: ";
		for (int i = 0; i < FRAMESIZE; i++) {
			cout << frames[i] << " ";
		}
		cout << endl;

	}

	int FRAMESIZE = 100; //
	int usedframes = 0; //ͳ���Ѿ�ʹ�õ�frames����
	int pageFault = 0; //
	MYTYPE *frames = new MYTYPE[FRAMESIZE];//��¼����frame�е�ҳ��

};

class CounterImplementation :public LRU {
public:
	CounterImplementation(int f) {
		FRAMESIZE = f;
		cout << "Counter Implementation==================" << endl;
		for (int i = 0; i < FRAMESIZE; i++) {
			frames[i] = 0;
			framesClock[i] = 0;
		}
	}

	~CounterImplementation() {
		delete[] framesClock;
		delete[]frames;
	}

	void action(MYTYPE current, int syscounter) {
		//search the memory
		bool searchResult = false;
		for (int i = 0; i<FRAMESIZE; i++) {
			if (frames[i] == current) {
				framesClock[i] = syscounter;
				searchResult = true;
				break;
			}
		}
		//if dont find in the memory,then pagefalut
		if (!searchResult) {
			pageFault++;

			int longest = INT_MAX;
			int victim = -1;
			//find the page in the memory 
			for (int i = 0; i < FRAMESIZE; i++) {
				if (framesClock[i] == 0) { //if there exists empty frames
					victim = i;
					break;
				}
				if (framesClock[i] <longest) {
					victim = i;
					longest = framesClock[i];
				}
			}
			//
			frames[victim] = current; //new page change the oldest page
			framesClock[victim] = syscounter;//
		}
		print("Counter implementation ");
	}

private:
	int *framesClock = new int[FRAMESIZE];
};

class ReferenceBitImplementation :public LRU {
public:
	ReferenceBitImplementation(int f) {
		FRAMESIZE = f;
		cout << "ReferenceBit Implementation========================" << endl;
		for (int i = 0; i < FRAMESIZE; i++)
			frames[i] = 0;
	}

	~ReferenceBitImplementation() {
		delete[] RefBit;
		delete[]frames;
	}

	void action(MYTYPE current) {
		//search the memory
		bool searchResult = false;
		for (int i = 0; i<FRAMESIZE; i++) {
			if (frames[i] == current) {
				searchResult = true;
				RefBit[i] = RefBit[i] | 128; //
				break;
			}
		}
		//if dont find in the memory,then pagefalut
		if (!searchResult) {
			pageFault++;

			if (usedframes < FRAMESIZE) {
				frames[usedframes] = current;

				RefBit[usedframes] = 128; //
				usedframes++;
			}
			else {
				unsigned char min = 255;
				int victim = -1;
				//find the page in the memory 
				for (int i = 0; i < FRAMESIZE; i++) {

					if (RefBit[i] <min) {
						min = RefBit[i];
						victim = i;
					}
				}
				frames[victim] = current; //new page change the oldest page
				RefBit[victim] = 128;
			}
		}
		print("RefBit implementation ");
		//printRefBit();
	}

private:
	unsigned char *RefBit = new unsigned char[FRAMESIZE];//����λ
	int period = 4;

	void printRefBit() {
		cout << "current Ref Bit" << endl;
		for (int i = 0; i < FRAMESIZE; i++)
			change(RefBit[i]);
		cout << "***************" << endl;
	}

	inline void change(char c)
	{
		for (int i = 7; i >= 0; i--)
			cout << ((c >> i) & 1) << ' ';
		cout << endl;
	}

	void MoveRight() {
		for (int i = 0; i < FRAMESIZE; i++)
			RefBit[i] = (RefBit[i] >> 1);
	}
};

class SecondChanceFIFO :public LRU {
public:
	SecondChanceFIFO(int f) {
		cout << "SecondChangeFIFO Implementation====================" << endl;
		FRAMESIZE = f;
		for (int i = 0; i < FRAMESIZE; i++)
			frames[i] = 0;
	}
	~SecondChanceFIFO() {
		delete[] framesRef;
		delete[]frames;
	}

	void action(MYTYPE current) {
		//search the memory
		bool searchResult = false;

		for (int i = 0; i < FRAMESIZE; i++) { //find ,do nothing
			if (frames[i] == current) {
				searchResult = true;
				break;
			}
		}
		//if dont find in the memory,then pagefalut
		if (!searchResult) {
			pageFault++;

			if (usedframes < FRAMESIZE) { //do not need replacement
				frames[usedframes] = current;
				framesRef[usedframes] = 1;
				Stack.push_back(current);
				usedframes++;
			}
			else { //we need reply a page
				int victim = -1; //���滻��frame��λ��
				for (;;) {
					//ѭ��FIFO��ֵ������Ƿ�refΪ0.�ǵĻ���ѡ��victim����ѯframes���ҵ�λ��i���滻��current
					MYTYPE val = Stack.front();
					int cur = -1;//��ǰֵ��frames�е�λ��
					for (int i = 0; i < FRAMESIZE; i++) {
						if (frames[i] == val)
							cur = i;
					}
					if (framesRef[cur] == 0) {//�����λframes������λ��0����ѡ��Ϊvictim
						victim = cur;
						Stack.remove(val);
						break;
					}
					else {
						framesRef[cur] = 0;
						Stack.remove(val);
						Stack.push_back(val);
					}
				}
				frames[victim] = current;
				framesRef[victim] = 1;
				Stack.push_back(current);
			}
		}
		print("2nd chance FIFO ");
	}

private:
	int * framesRef = new int[FRAMESIZE];
	list<MYTYPE> Stack; //���ڼ�¼FIFO˳��
};

class SecondChanceClock :public LRU {
public:
	SecondChanceClock(int f) {
		cout << "SecondChange Clock Implementation====================" << endl;
		FRAMESIZE = f;
		for (int i = 0; i < FRAMESIZE; i++)
			frames[i] = 0;
	}
	~SecondChanceClock() {
		delete[] framesRef;
	}
	void action(MYTYPE current) {
		//search the memory
		bool searchResult = false;

		for (int i = 0; i < FRAMESIZE; i++) { //find ,do nothing
			if (frames[i] == current) {
				searchResult = true;
				break;
			}
		}
		//if dont find in the memory,then pagefalut
		if (!searchResult) {
			pageFault++;

			if (usedframes < FRAMESIZE) { //do not need replacement
				frames[usedframes] = current;
				framesRef[usedframes] = 1;
				usedframes++;
			}
			else { //we need reply a page
				int victim = -1;
				for (;;) {
					if (framesRef[point] == 0) {
						victim = point;
						point = (point + 1) % FRAMESIZE;
						break;
					}
					else {
						framesRef[point] = 0;
						point = (point + 1) % FRAMESIZE;
					}
				}
				frames[victim] = current;
				framesRef[victim] = 1;
			}
		}
		print("2nd chance Clock");
	}

private:
	int * framesRef = new int[FRAMESIZE];
	int point = 0; //ѭ������ָ��
};

class StackImplementation :public LRU {
public:
	StackImplementation(int f) {
		cout << "Stack Implementation ========================" << endl;
		FRAMESIZE = f;
	}

	void action(MYTYPE current) {
		//search the memory
		bool searchResult = false;
		list<MYTYPE>::iterator itor;  //���������
		itor = MyStack.begin();
		while (itor != MyStack.end())
		{
			if (current == *itor++) {
				//each reference, move to the top
				MyStack.remove(current);
				MyStack.push_back(current);
				searchResult = true;
				break;
			}
		}
		//if dont find in the memory,then pagefalut
		if (!searchResult) {
			pageFault++;
			if (usedframes < FRAMESIZE) {//frames are not full, 
				MyStack.push_back(current);
				usedframes++;
			}
			else { //frames is full
				MYTYPE victim = getLRU();
				MyStack.remove(victim);
				MyStack.push_back(current);
			}
		}
		printList();
	}

private:
	list<MYTYPE> MyStack;

	MYTYPE getLRU() {
		return MyStack.front(); //return the element at the bottom of the stack
	}

	void printList() {
		cout << "stack implementation current frames: ";
		list<MYTYPE>::iterator itor;  //���������
		itor = MyStack.begin();
		while (itor != MyStack.end())
		{
			cout << *itor++ << " ";
		}
		cout << endl;
	}
};

//��ȡ����,���������
void loadtrace(string path, queue<string> &data) {

	ifstream fin(path);
	if (!fin.is_open()) {
		cout << "error open" << endl;
		return;
	}

	char *temp;
	string addr;
	char line[200];
	while (fin.getline(line, 200))
	{
		temp = strtok(line, " ");
		for (int i = 0; i < 9; i++)
			temp = strtok(NULL, " ");
		//cout << temp << endl;
		addr = temp;
		if (addr != "0") {
			//	cout << addr << endl;
			data.push(addr);
		}
	}
}


int main() {

	ofstream fout("out.txt");
	if (!fout.is_open()) {
		cout << "error!" << endl;
		return 0;
	}

	int type, maxframe;
	cout << "Please enter a number: 1-random number  2-1K trace  3-10M trace" << endl;
	cin >> type;
	cout << "Please enter the max frames " << endl;
	cin >> maxframe;

	double counterpf, stackpf, refbitpf, fifo2pf, clock2pf;

	double t1, t2, t3, t4, t5;//��¼ÿ���㷨����ʱ��

	if (type == 1) {
		for (int i = 1; i <= maxframe; i++) {
			//��ÿ��framesֵ��������Ӧ����
			int framesize = i;
			CounterImplementation counter(framesize);
			StackImplementation stack(framesize);
			ReferenceBitImplementation refbit(framesize);
			SecondChanceFIFO fifo2(framesize);
			SecondChanceClock clock2(framesize);

			int syscounter = 0;
			t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;
			while (syscounter<TESTSIZE) {
				syscounter++; //system clock
				int current = (rand() % PAGENUM) + 1;
				cout << "system counter *************** " << syscounter << "********" << endl;
				cout << "current pages: " << current << " " << endl;

				clock_t s1 = clock();
				counter.action(current, syscounter);
				clock_t s2 = clock();

				stack.action(current);
				clock_t s3 = clock();

				refbit.action(current);
				clock_t s4 = clock();

				fifo2.action(current);
				clock_t s5 = clock();

				clock2.action(current);
				clock_t s6 = clock();

				t1 = ((double)(s2 - s1) / CLOCKS_PER_SEC) * 1000 + t1;
				t2 = ((double)(s3 - s2) / CLOCKS_PER_SEC) * 1000 + t2;
				t3 = ((double)(s4 - s3) / CLOCKS_PER_SEC) * 1000 + t3;
				t4 = ((double)(s5 - s4) / CLOCKS_PER_SEC) * 1000 + t4;
				t5 = ((double)(s6 - s5) / CLOCKS_PER_SEC) * 1000 + t5;

			}
			counterpf = double(counter.getPageFault()) / TESTSIZE;
			stackpf = double(stack.getPageFault()) / TESTSIZE;
			refbitpf = double(refbit.getPageFault()) / TESTSIZE;
			fifo2pf = double(fifo2.getPageFault()) / TESTSIZE;
			clock2pf = double(clock2.getPageFault()) / TESTSIZE;

			cout << "**********************\n**********************\n**********************\n**********************\n";
			cout << "Counter Implementation page faults:  " << counterpf << endl;
			cout << "Stack Implementation page faults:  " << stackpf << endl;
			cout << "Reference Bit Implementation page faults:  " << refbitpf << endl;
			cout << "Second Chance FIFO Implementation page faults:  " << fifo2pf << endl;
			cout << "Second Chance Clock Implementation page faults:  " << clock2pf << endl;
			cout << "time" << endl;
			cout << t1 << '\t' << t2 << '\t' << t3 << '\t' << t4 << '\t' << t5 << endl;
			cout << "**********************\n**********************\n**********************\n**********************\n";
			fout << framesize << '\t' << counterpf << '\t' << stackpf << '\t' << refbitpf << '\t' << fifo2pf << '\t' << clock2pf;
			fout << "\ttime" << '\t' << t1 << '\t' << t2 << '\t' << t3 << '\t' << t4 << '\t' << t5 << endl;
		}
		fout.close();
	}

	system("pause");
	return 0;
}

