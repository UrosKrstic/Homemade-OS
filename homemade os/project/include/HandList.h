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
    SignalHandlerList(const SignalHandlerList& shl);
    SignalHandlerList& operator=(const SignalHandlerList& shl);
    ~SignalHandlerList();
    void insertHandler(SignalHandler sigHandler);
    SignalHandler begin() volatile;
    SignalHandler end() volatile;
    SignalHandler getNext() volatile;
    void swap(SignalHandler sig1, SignalHandler sig2);
    void clearList();
};

#endif //_HANDLIST_H_