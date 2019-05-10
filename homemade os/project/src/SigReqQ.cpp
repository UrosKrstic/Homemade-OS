#include "SigReqQ.h"
#include "PCB.h"

SignalRequestQueue::SignalRequestQueue() {
    head = tail = nullptr;
}

SignalRequestQueue::~SignalRequestQueue() {
    Node* tmp = head;
    while (tmp != nullptr) {
        head = head->next;
        delete tmp;
        tmp = head;
    }
}

void SignalRequestQueue::saveRequest(int signalID) {
    if (head == nullptr) {
        head = tail = new Node(signalID, nullptr);
    }
    else {
        tail = tail->next = new Node(signalID, nullptr);
    }
}

int SignalRequestQueue::takeRequest() {
    if (head == nullptr) {
        return -1;
    }
    else {
        int return_value = head->signalID;
        head = head->next;
        if (head == nullptr) tail = nullptr;
        return return_value;
    }
}