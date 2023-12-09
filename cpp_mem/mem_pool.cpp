#include <iostream>
using namespace std;

// https://blog.csdn.net/K346K346/article/details/49538975

template<int ObjectSize, int NumofObjects = 20>
class MemPool {
private:
	//空闲节点结构体
	struct FreeNode{
		FreeNode* pNext;
		char data[ObjectSize];
	};

	//内存块结构体
	struct MemBlock{
		MemBlock* pNext;
		FreeNode data[NumofObjects];
	};

	FreeNode* freeNodeHeader;
	MemBlock* memBlockHeader;

public:
	MemPool() {
		freeNodeHeader = NULL;
		memBlockHeader = NULL;
        cout<<"init memPool" <<endl;
	}

	~MemPool() {
		MemBlock* ptr;
		while (memBlockHeader)
		{
            cout<<"decons MemPool Block" <<endl;
			ptr = memBlockHeader->pNext;
			delete memBlockHeader;
			memBlockHeader = ptr;
		}
	}
	void* malloc();
	void free(void*);
};

// 分配空闲的结点。
template<int ObjectSize, int NumofObjects>
void* MemPool<ObjectSize, NumofObjects>::malloc(){
	//无空闲节点，申请新内存块
	if (freeNodeHeader == NULL){
        cout<<"no free block make new block" <<endl;
		MemBlock* newBlock = new MemBlock;
		newBlock->pNext = NULL;

		freeNodeHeader=&newBlock->data[0];	 //设置内存块的第一个节点为空闲节点链表的首节点
		//将内存块的其它节点串起来
		for (int i = 1; i < NumofObjects; ++i) {
			newBlock->data[i - 1].pNext = &newBlock->data[i];
		}
		newBlock->data[NumofObjects - 1].pNext=NULL;

		// 首次申请内存块
		if (memBlockHeader == NULL) {
			memBlockHeader = newBlock;
		} else {
			// 将新内存块加入到内存块链表。
			newBlock->pNext = memBlockHeader;
			memBlockHeader = newBlock;
		}
	}
	// 返回空节点闲链表的第一个节点。
	void* freeNode = freeNodeHeader;
	freeNodeHeader = freeNodeHeader->pNext;
    cout<<"after alloc mem" <<endl;
	return freeNode;
}

// 释放已经分配的结点。
template<int ObjectSize, int NumofObjects>
void MemPool<ObjectSize, NumofObjects>::free(void* p) {
	FreeNode* pNode = (FreeNode*)p;
	pNode->pNext = freeNodeHeader;	//将释放的节点插入空闲节点头部
	freeNodeHeader = pNode;
}

class ActualClass {
	static int count;
	int No;

public:
	ActualClass() {
        cout<< "ActualClass construct" <<endl;
		No = count;
		count++;
	}
    ~ActualClass(){
        cout<<"ActualClass Deconstruct" <<endl;
    }

	void print() {
		cout << this << ": ";
		cout << "the " << No << "th object" << endl;
	}

	void* operator new(size_t size);
	void operator delete(void* p);
};

// 定义内存池对象
MemPool<sizeof(ActualClass), 5> mp;

void* ActualClass::operator new(size_t size) {
	return mp.malloc();
}

void ActualClass::operator delete(void* p) {
	mp.free(p);
}

int ActualClass::count = 0;

int main() {
    
    // 栈上的对象生命周期是{}内
    {
        ActualClass p0;
        p0.print();
        cout<<"after p0" <<endl;
    }

    // 先分调用重载的new 操作符，分配内存
    // 再调用 ActualClass的构造函数构造对象
	ActualClass* p1 = new ActualClass;
	p1->print();

	ActualClass* p2 = new ActualClass;
	p2->print();
	delete p1;

    // 不会重新分配内存
	p1 = new ActualClass;
	p1->print();

    ActualClass* p3 = new ActualClass;
    p3->print();

    ActualClass* p4 = new ActualClass;
    p4->print();

    ActualClass* p5 = new ActualClass;
    p5->print();

    ActualClass* p6 = new ActualClass;
    p6->print();

    ActualClass* p7 = new ActualClass;
    p7->print();

    delete p1;
    delete p2;
    delete p3;
    delete p4;
    delete p5;
    delete p6;
    delete p7;
}
