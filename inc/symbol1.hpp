#ifndef SYMBOL_H
#define SYMBOL_H

#include <iostream>
#include "shared.hpp"

using namespace std;

class Symbol
{

private:
    int ID;
    unsigned int Value;
    string Name;
    string Type; //Local , Global, Extern
    int SectionID;
    string SectionName;
    //bool isSection; // moze i samo da se provera ne mora da se salje
    bool isDefined;
    /* data */

public:
    Symbol(int ID, unsigned int Value, string Name, string Type, int SectionID, string SectionName, bool isDefined) {
        this->ID = ID;
        this->Value = Value;
        this->Name = Name;
        this->Type = Type;
        this->SectionID = SectionID;
        this->SectionName = SectionName;
        this->isDefined = isDefined;
    }

    friend ostream& operator<<(ostream& os, shared_ptr<Symbol> symb){
        os << symb->getID();
        os << addSpace(symb->getID());

        os << "|" << symb->getName();
        if(symb->getName().length() < 10) {
            for(int i = symb->getName().length(); i < 10; i++)    os << " ";
        }
        os << "|" << (symb->getSectionID() == symb->getID() ? " Section  |" : " Symbol   |");
        os << symb->getValue() << addSpace(symb->getValue()) << "|";
        os << symb->getSectionID() << addSpace(symb->getSectionID());
        os << " |" << symb->getisDefined() << "        ";
        os << "|  " <<  symb->getType() << "   \n";
    }

    void setID(int ID) {
        this->ID = ID;
    }

    int getID() {
        return ID; 
    }

    void setValue(int Value) {
        this->Value = Value;
    }

    int getValue() {
        return Value; 
    }

    void setName(string Name) {
        this->Name = Name;
    }

    string getName() {
        return this->Name; 
    }

    void setType(string Type) {
        this->Type = Type;
    }

    string getType() {
        return Type; 
    }

    void setSectionID(int SectionID) {
        this->SectionID = SectionID;
    }

    int getSectionID() {
        return SectionID; 
    }

    void setSectionName(string SectionName) {
        this->SectionName = SectionName;
    }

    string getSectionName() {
        return SectionName; 
    }

    bool isSection() {
        return this->SectionName == this->Name; 
    }

    void setisDefined(bool isDefined) {
        this->isDefined = isDefined;
    }

    bool getisDefined() {
        return isDefined; 
    }

    void incrementValue(int Num) {
        this->Value += Num;
    }

};



#endif