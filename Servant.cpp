#include "Servant.h"

Servant::Servant(ConcurrentLinkedList &l, ConcurrentBag &bag) : l(l), bag(bag), numPresents(0), numThankyous(0) {}

// Take a present out of the bag and return it
int Servant::getPresentFromBag() {
    auto tag = bag.pop();
    if (tag == -1)
        return false;
    numPresents++;
    return l.add(tag);
}

// Search for a present with the given tag in the list, remove it, and write a thank you note
bool Servant::writeThankyou(int tag) {
    if (tag == -1)
        return false;
    bool result = l.remove(tag);
    numThankyous++;
    return result;
}

int Servant::getNumPresents() { return numPresents; }

int Servant::getNumThankyous() { return numThankyous; }

int Servant::getBagSize() {return bag.getSize(); }