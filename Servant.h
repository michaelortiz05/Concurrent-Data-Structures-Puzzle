#ifndef SERVANT_H
#define SERVANT_H

#include "ConcurrentDataStructures.h"

// The servant class has variables representing the number of presents/thank you notes that this servant has done
// It has a refrence to the same linkedlist and bag that all the other servants do
class Servant {
    private:
        ConcurrentLinkedList &l;
        ConcurrentBag &bag;
        int numPresents;
        int numThankyous;
    public:
        Servant(ConcurrentLinkedList &l, ConcurrentBag &bag);
        int getPresentFromBag(); 
        int getNumPresents();
        int getNumThankyous();
        bool writeThankyou(int tag); 
        int getBagSize(); 
};

#endif