#ifndef _SIGREQQ_H_
#define _SIGREQQ_H_

class SignalRequestQueue {
    struct Node {
        int signalID;
        Node* next;
        Node(int _signalID, Node* _next) : signalID(_signalID), next(_next) {}
    };
    Node *head, *tail;
public:
    SignalRequestQueue();
    ~SignalRequestQueue();
    void saveRequest(int signalID);
    int takeRequest();
};

#endif //_SIGREQQ_H_