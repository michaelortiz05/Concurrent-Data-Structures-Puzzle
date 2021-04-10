#ifndef CONCURRENT_LINKED_LIST_H
#define CONCURRENT_LINKED_LIST_H

#include <thread>
#include <mutex>
#include <deque>

// A node in the concurrent linked list
struct Node {
    int data;
    Node* next = nullptr;
    std::mutex mtx;
};

// The temperatureReading struct
struct TempReading {
    int temperature;
    int minute;
};

// Optimistic synchronization linked list supporting add(), remove(), contains(), validate()
class ConcurrentLinkedList {
    private:
        Node* head;
    public:
        ConcurrentLinkedList();
        ~ConcurrentLinkedList();
        bool add(int item);
        bool remove(int item);
        bool contains(int item);
        bool validate(Node* pred, Node* curr);
};

// Concurrent bag for the minotaur simulation. Supports random O(1) access to elements and always removes them once accessed
class ConcurrentBag {
    private: 
        int size;
        std::deque<int> bag;
        std::mutex mtx;
    public:
        ConcurrentBag(int size);
        int pop();
        int getSize();
        bool isEmpty();
};

// Modification to deque to support locking
class LockDeque {
    private:
        std::mutex mtx;
    public:
        std::deque<TempReading*> readings;
        void push_back(TempReading* reading);
        TempReading* pop_front();
        int size();
        void printDeque();
};

#endif