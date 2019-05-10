#include "HandList.h"
#include "PCB.h"

SignalHandlerList::SignalHandlerList() {
    head = tail = tmp = nullptr;
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

SignalHandler SignalHandlerList::begin() {
    tmp = head;
    if (head != nullptr)
        return tmp->signalHandler;
    else
        return nullptr;
}

SignalHandler SignalHandlerList::end() {
    return nullptr;
}

SignalHandler SignalHandlerList::getNext() {
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

