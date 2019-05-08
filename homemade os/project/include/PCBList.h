#ifndef _PCBLIST_H_
#define _PCBLIST_H_
#include "PCB.h"


class PCBList {
	struct Node {
		PCB* pcb;
		Node *next;
		Node(PCB* pcb, Node* next) {
			this->pcb = pcb;
			this->next = next;
		}
        Node(const Node& n) {
			pcb = n.pcb;
			next = n.next;
		}
	};
	Node *head, *tail;
public:
	PCBList();
	~PCBList();
	void insert(PCB* pcb);
	PCB* remove();
	PCB* remove(unsigned id);
	PCB* searchById(unsigned id);
    void printList();
};

extern PCBList* GlobalPCBList;

#endif //_PCBLIST_H_