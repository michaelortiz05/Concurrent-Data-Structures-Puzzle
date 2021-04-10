#include "ConcurrentDataStructures.h"
#include <bits/stdc++.h>
#include <algorithm>
#include <random>

using namespace std;

// Constructor initializes sentinel nodes
ConcurrentLinkedList::ConcurrentLinkedList() { 
    head = new Node;
    head->data = INT_MIN;
    head->next = new Node;
    head->next->data = INT_MAX;
}

// Destructor frees all memory
ConcurrentLinkedList::~ConcurrentLinkedList() {
    Node* current = head;
    Node* next = nullptr;
    while (current != nullptr) {
        next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
}

// Remove is based on optimistic synchronization from the textbook
bool ConcurrentLinkedList::remove(int data) {
    bool success = true;

    while (true) {
        Node* pred = head;
        Node* curr = pred->next;

        while (curr->data < data) {
            pred = curr;
            curr = curr->next;
        }

        pred->mtx.lock();
        curr->mtx.lock();

        if (validate(pred, curr)) {
            if (curr->data == data) {
                pred->next = curr->next;

                pred->mtx.unlock();
                curr->mtx.unlock();
            }
            else {
                pred->mtx.unlock();
                curr->mtx.unlock();
                success = false;
            }

            return success;
        }
        pred->mtx.unlock();
        curr->mtx.unlock();
    }
}

// add is based on optimistic synchronization from the textbook
bool ConcurrentLinkedList::add(int data) {
    while (true) {
        bool success = true;
        Node* pred = head;
        Node* curr = pred->next;

        while (curr->data <= data) {
            pred = curr;
            curr = curr->next;
        }

        pred->mtx.lock();
        curr->mtx.lock();

        if (validate(pred, curr)) {
            if(curr->data) success = false;

            else {
                Node* newNode = new Node;
                newNode->next = curr;
                pred->next = newNode;
            }

            pred->mtx.unlock();
            curr->mtx.unlock();

            return success;   
        }
        pred->mtx.unlock();
        curr->mtx.unlock();
    } 
}

bool ConcurrentLinkedList::validate(Node* pred, Node* curr) {
    Node* node = head;
    while (node->data <= pred->data) {
        if (node == pred) 
            return pred->next == curr;
        node = node->next;  
    }
    return false;
}

//  contains is based on optimistic synchronization from the textbook
bool ConcurrentLinkedList::contains(int data) {
    Node* pred = nullptr;
    Node* curr = nullptr;

    head->mtx.lock();
    pred = head;
    curr = pred->next;
    curr->mtx.lock();

    while (curr->data < data) {
        pred->mtx.unlock();
        pred = curr;
        curr = curr->next;
        curr->mtx.lock();
    }

    if (curr->data == data) {
        curr->mtx.unlock();
        pred->mtx.unlock();
        return true;
    }

    curr->mtx.unlock();
    pred->mtx.unlock();
    return false;
}

// Simulate the bag by shuffling a deque of unique elements from 1 - 500k
ConcurrentBag::ConcurrentBag(int size) : size(size) {
    mtx.lock();
    for (auto i = 0; i < size; i++)
        bag.push_back(i);
    srand(time(0));
    random_shuffle(begin(bag), end(bag));
    mtx.unlock();
}

bool ConcurrentBag::isEmpty() { return size <= 0; }

// Since the bag is shuffled, simply pop the first element to simulate randomly pulling a present from the bag
int ConcurrentBag::pop() {
    if (isEmpty()) return -1;

    mtx.lock();
    auto element = bag.front();
    bag.pop_front();
    size--;
    mtx.unlock();

    return element;
}

int ConcurrentBag::getSize() { return size; }

// add locking to deque push_back
void LockDeque::push_back(TempReading* reading) {
    lock_guard<mutex> lg(mtx);
    readings.push_back(reading);
}

// pop and return value with locking
TempReading* LockDeque::pop_front() {
    lock_guard<mutex> lg(mtx);
    auto reading = readings.front();
    readings.pop_front();
    return reading;
}

int LockDeque::size() { return readings.size(); }

// for testing, not used in implementation
void LockDeque::printDeque() {
    for (auto tr : readings) {
        cout << "t: " << tr->temperature << "\tm: " << tr->minute << endl;
    }
}


