#ifndef _HANDLIST_H_
#define _HANDLIST_H_

#include "thread.h"

class SignalHandlerList {
    struct Node {
        SignalHandler signalHandler;
        Node* next;
        Node(SignalHandler _sigHand, Node * _next) : signalHandler(_sigHand), next(_next) {}
    };
    Node *head, *tail, *tmp;
public:
    SignalHandlerList();
    ~SignalHandlerList();
    void insertHandler(SignalHandler sigHandler);
    SignalHandler begin();
    SignalHandler end();
    SignalHandler getNext();
    void swap(SignalHandler sig1, SignalHandler sig2);
    void clearList();
};

#endif //_HANDLIST_H_