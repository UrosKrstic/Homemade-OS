#include "PCBList.h"
#include "timer.h"
#include <iostream.h>


PCBList::PCBList() {
	head = tail = nullptr;
}

PCBList::~PCBList() {
	Node *tmp = head;
	while (tmp != nullptr) {
		tmp = tmp->next;
		delete head;
		head = tmp;
	}
}

void PCBList::insert(PCB* pcb) {
	if (head == nullptr) {
		head = tail = new Node(pcb, nullptr);
	}
	else {
		tail = tail->next = new Node(pcb, nullptr);
	}
}

PCB* PCBList::searchById(unsigned id) {
	Node *tmp = head;
	while (tmp != nullptr) {
		if (tmp->pcb->getID() == id) {
			return tmp->pcb;
		}
		tmp = tmp->next;
	}
	return nullptr;
}

PCB* PCBList::remove() {
	PCB* pcb = nullptr;
	if (head != nullptr) {
		pcb = head->pcb;
		Node *tmp = head;
		head = head->next;
		tmp->next = nullptr;
		delete tmp;
		if (head == nullptr) tail = nullptr;
	}
    return pcb;
}

PCB* PCBList::remove(unsigned id) {
	Node *tmp = head, *prev = nullptr;
	while (tmp != nullptr) {
		if (tmp->pcb->getID() == id) {

			PCB* pcb = tmp->pcb;

			if (prev != nullptr) {
				prev->next = tmp->next;
				if (tail == tmp) tail = prev;
			}
			else {
				head = head->next;
				if (head == nullptr) tail = nullptr;	
			}

            tmp->next = nullptr;
			delete tmp;
			return pcb;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return nullptr;
}

void PCBList::printList() {
        Node* tmp = head;
        while (tmp != (void*)0) {
            cout << "Thread with ID:" << tmp->pcb->getID() << endl;
            tmp = tmp->next;
        }
    }