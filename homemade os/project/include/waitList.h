#ifndef _WAITLIST_H_
#define _WAITLIST_H_
#include "PCB.h"


class WaitList {
	friend class KernelSem; 
	friend class SemList;
	struct Node {
        unsigned wait_time;
		PCB* pcb;
		Node *next;
		Node(PCB* pcb, unsigned wait_time,Node* next) {
			this->pcb = pcb;
            this->wait_time = wait_time;
			this->next = next;
		}
        Node(const Node& n) {
			pcb = n.pcb;
            this->wait_time = wait_time;
			next = n.next;
		}
	};
    Node *head, *tail;
public:
	WaitList();
	~WaitList();
	void insert(PCB* pcb, unsigned wait_time);
	PCB* remove();
	PCB* remove(unsigned id);
	PCB* searchById(unsigned id);
    void printList();
};


#endif //_WAITLIST_H_