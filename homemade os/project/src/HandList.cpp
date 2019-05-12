#include "HandList.h"
#include "PCB.h"
#include <iostream.h>

SignalHandlerList::SignalHandlerList() {
    head = tail = tmp = nullptr;
}

SignalHandlerList::SignalHandlerList(const SignalHandlerList& shl) {
     if (this != &shl) {
        head = tail = tmp = nullptr;
        SignalHandlerList::Node* temp = shl.head;
        for(; temp != nullptr; temp = temp->next) {
            insertHandler(temp->signalHandler);
        }
    }
}

SignalHandlerList& SignalHandlerList::operator=(const SignalHandlerList& shl) {
    if (this != &shl) {
        clearList();
        head = tail = tmp = nullptr;
        SignalHandlerList::Node* temp = shl.head;
        for(; temp != nullptr; temp = temp->next) {
            insertHandler( temp->signalHandler);
        }
    }
    return *this;
}

SignalHandlerList::~SignalHandlerList() {
    tmp = head;
    while (tmp != nullptr) {
        head = head->next;
        delete tmp;
        tmp = head;
    }
}

void SignalHandlerList::insertHandler(SignalHandler sigHandler) {
    if (head == nullptr) {
        head = tail = new Node(sigHandler, nullptr);
    }
    else {
        tail = tail->next = new Node(sigHandler, nullptr);
    }
}

SignalHandler SignalHandlerList::begin()  volatile {
    tmp = head;
    if (head != nullptr)
        return tmp->signalHandler;
    else
        return nullptr;
}

SignalHandler SignalHandlerList::end()  volatile {
    return nullptr;
}

SignalHandler SignalHandlerList::getNext() volatile {
    if (tmp != nullptr) {
        tmp = tmp->next;
        if (tmp != nullptr) {
            return tmp->signalHandler;
        }
    }
    return nullptr;
}

void SignalHandlerList::swap(SignalHandler hand1, SignalHandler hand2) {
    Node *left = nullptr, *right = nullptr;
    for (tmp = head; tmp != nullptr; tmp = tmp->next) {
        if (tmp->signalHandler == hand1) {
            left = tmp;
        }
        else if (tmp->signalHandler == hand2) {
            right = tmp;
        }
        if (left != nullptr && right != nullptr) {
            SignalHandler tmpHand = left->signalHandler;
            left->signalHandler = right->signalHandler;
            right->signalHandler = tmpHand;
            break;
        }
    }
}

void SignalHandlerList::clearList() {
    tmp = head;
    while (tmp != nullptr) {
        head = head->next;
        delete tmp;
        tmp = head;
    }
    head = tail = tmp = nullptr;
}
