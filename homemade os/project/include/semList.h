#ifndef _SEMLIST_H_
#define _SEMLIST_H_
#include "kerSem.h"


class SemList {
	struct Node {
		KernelSem* sem;
		Node *next;
		Node(KernelSem* sem, Node* next) {
			this->sem = sem;
			this->next = next;
		}
        Node(const Node& n) {
			sem = n.sem;
			next = n.next;
		}
	};
	Node *head, *tail;
public:
	SemList();
	~SemList();
	void insert(KernelSem* sem) volatile;
	KernelSem* searchById(unsigned id);
	void update() volatile;
    KernelSem* remove(unsigned id) volatile;
	KernelSem* remove();
    void printList() volatile;
};

extern volatile SemList *GlobalSemaphoreList;

#endif //_SEMLIST_H_