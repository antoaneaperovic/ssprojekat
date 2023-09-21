#ifndef POOL_ITEM_H
#define POOL_ITEM_H

#include <iostream>

using namespace std;

class PoolItem {
    private: 
    string Name;
    unsigned int Offset;

    public:

    PoolItem(string Name, unsigned int Offset) {
        this->Name = Name;
        this->Offset = Offset;
    }

    void setValue(unsigned int Offset) {
        this->Offset = Offset;
    }

    unsigned int getValue() {
        return Offset; 
    }

    void setName(string Name) {
        this->Name = Name;
    }

    string getName() {
        return Name; 
    }

    void addToOffset(unsigned int Value) {
        this->Offset += Value;
    }

};

#endif