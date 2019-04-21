#include "semList.h"
#include "timer.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "waitList.h"
#include <iostream.h>

SemList::SemList() {
	head = tail = nullptr;
}

SemList::~SemList() {
	Node *tmp = head;
	while (tmp != nullptr) {
		tmp = tmp->next;
		delete head;
		head = tmp;
	}
}

void SemList::insert(KernelSem* sem) volatile {
	if (head == nullptr) {
		head = tail = new Node(sem, nullptr);
	}
	else {
		tail = tail->next = new Node(sem, nullptr);
	}
}

KernelSem* SemList::searchById(unsigned id) {
	Node *tmp = head;
	while (tmp != nullptr) {
		if (tmp->sem->getID() == id) {
			return tmp->sem;
		}
		tmp = tmp->next;
	}
	return nullptr;
}

void SemList::update() volatile {
    Node *node = head;
    while (node != nullptr) {
        WaitList::Node *tmp = node->sem->sleepList->head, *prev = nullptr;
        while (tmp != nullptr) {
            if (--tmp->wait_time == 0) {
				node->sem->sem_value++;
                PCB* pcb = tmp->pcb;

                if (prev != nullptr) {
                    prev->next = tmp->next;
                    if (node->sem->sleepList->tail == tmp) 
						node->sem->sleepList->tail = prev;
                }
                else {
                    node->sem->sleepList->head = node->sem->sleepList->head->next;
                    if (node->sem->sleepList->head == nullptr) 
						node->sem->sleepList->tail = nullptr;	
                }

                tmp->next = nullptr;
				WaitList::Node *tmp2 = tmp;
				tmp = tmp->next;
                delete tmp2;

                pcb->status &= ~PCB_BLOCKED;
                pcb->status |= PCB_READY | PCB_TIMEOUT_DEBLOCK;
                Scheduler::put(pcb);
            }
			else {
				prev = tmp;
            	tmp = tmp->next;
			}
        }
        node = node->next;
    }
}

KernelSem* SemList::remove() {
	KernelSem* sem = nullptr;
	if (head != nullptr) {
		sem = head->sem;
		Node *tmp = head;
		head = head->next;
		tmp->next = nullptr;
		delete tmp;
		if (head == nullptr) tail = nullptr;
	}
    return sem;
}

KernelSem* SemList::remove(unsigned id) volatile {
	Node *tmp = head, *prev = nullptr;
	while (tmp != nullptr) {
		if (tmp->sem->getID() == id) {

			KernelSem* sem = tmp->sem;

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
			return sem;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return nullptr;
}

void SemList::printList() volatile {
   	Node* tmp = head;
    while (tmp != (void*)0) {
        cout << "Semaphore with ID:" << tmp->sem->getID() << endl;
        tmp = tmp->next;
    }
}

volatile SemList *GlobalSemaphoreList;