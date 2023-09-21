#ifndef RELOCATION_ITEM_H
#define RELOCATION_ITEM_H

#include <iostream>

using namespace std;

class RelocationItem {
    private: 

    string Name;
    unsigned int Offset;
    unsigned int Addend;

    public:

    RelocationItem(string Name, unsigned int Offset, unsigned int Addend) {
        this->Name = Name;
        this->Offset = Offset;
        this->Addend = Addend;
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

    void setAddend(unsigned int Addend) {
        this->Addend = Addend;
    }

    unsigned int getAddend() {
        return Addend; 
    }
};

#endif