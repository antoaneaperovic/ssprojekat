#ifndef SECTION_H
#define SECTION_H

#include "pool_item.hpp"
#include "relocation_item.hpp"
#include "shared.hpp"
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Section {
    private:
    int ID;
    string Name;
    unsigned int Counter = 0;
    vector<shared_ptr<PoolItem>> SymbolItemsPool;
    vector<shared_ptr<PoolItem>> LiteralItemsPool;
    vector<shared_ptr<RelocationItem>> RelocationTableItems;
    unsigned int Size = 0;
    int newAddress = 0;
    bool hasPlaceAddress = false;
    vector<unsigned int> CodeInt;
    vector<string> CodeHex;

    public:
    Section(int ID, string Name, vector<shared_ptr<RelocationItem>> RelocationTableItems = {}, vector<string> CodeHex ={}) {
        this->ID = ID;
        this->Name = Name;
        this->RelocationTableItems = RelocationTableItems;
        this->CodeHex = CodeHex;
    }

    friend ostream& operator<<(ostream& os, shared_ptr<Section> sect){
        os << "\nName: " << sect->getName() << "    ID: " << sect->getID() << "    Size: " << sect->getSize() << "\n";

        os << "New address:   " << decToHex(sect->getNewAddress(), false) << endl;

        os << "Literals:\n" ;
        for(auto lit: sect->LiteralItemsPool) {
            os << "Offset:" << lit->getValue() << "        Value: " << lit->getName() << "\n";
        }

        os << "Symbols:\n" ;
        for(auto sym: sect->SymbolItemsPool) {
            os << "Offset:" << sym->getValue() << "        Name: " << sym->getName() << "\n";
        }

        os << "Relocation table:\n" ;
        for(auto rel: sect->RelocationTableItems) {
            os << "Name: " << rel->getName() << "     Offset:" << rel->getValue() << "    Addend:" << rel->getAddend() <<  "\n";
        }

        os << "CODE:\n" ;
        int i = 0;
        for(auto codeHex: sect->CodeHex) {
            os << codeHex <<  " ";
            if(i == 7 ) {
                os << "\n";
                i = 0; 
            }
            else i++;
        } 
    }

    int getID() {
	    return this->ID;
    }
    void setID(int ID) {
    	this->ID = ID;
    }

    int getNewAddress() {
	    return newAddress;
    }
    void setNewAddress(int nAddress) {
    	newAddress = nAddress;
    }

    bool getHasPlaceAddress() {
        return hasPlaceAddress;
    }

    void setHasPlaceAddress(bool hasPA) {
        hasPlaceAddress = hasPA;
    }

    unsigned getCounter() {
    	return this->Counter;
    }
    void setCounter(unsigned int Counter) {
    	this->Counter = Counter;
    }

    void incrementCounter(unsigned int Value) {
        this->Counter += Value;
    }

    string getName() {
    	return Name;
    }
    void setName(string Name) {
    	this->Name = Name;
    }

    vector<shared_ptr<PoolItem>> getSymbolItemsPool() {
    	return this->SymbolItemsPool;
    }
    void setSymbolItemsPool(vector<shared_ptr<PoolItem>> SymbolItemsPool = {}) {
    	this->SymbolItemsPool = SymbolItemsPool;
    }

    vector<shared_ptr<PoolItem>> getLiteralItemsPool() {
    	return this->LiteralItemsPool;
    }
    void setLiteralItemsPool(vector<shared_ptr<PoolItem>> LiteralItemsPool) {
    	this->LiteralItemsPool = LiteralItemsPool;
    }

    vector<shared_ptr<RelocationItem>> getRelocationTableItems() {
    	return this->RelocationTableItems;
    }
    void setRelocationTableItems(vector<shared_ptr<RelocationItem>> RelocationTableItems) {
    	this->RelocationTableItems = RelocationTableItems;
    }

    unsigned int getSize() {
    	return this->Size;
    }
    void setSize(unsigned int Size) {
    	this->Size = Size;
    }

    void incrementSize(unsigned int Value) {
        this->Size += Value;
    }

    vector<string> getCodeHex() {
	    return this->CodeHex;
    }
    void setCodeHex(vector<string> CodeHex) {
    	this->CodeHex = CodeHex;
    }

    void setCodeHexWithIndex(int index, string CodeHex) {
    	this->CodeHex[index] = CodeHex;
    }

    string getCodeHexWithIndex(int index) {
    	return this->CodeHex[index];
    }

     void addCodeHex(string Code) {
        CodeHex.push_back(Code);
    }

    vector<unsigned int> getCodeInt() {
	    return this->CodeInt;
    }
    void setCodeInt(int index, unsigned int CodeInt) {
    	this->CodeInt[index] = CodeInt;
    }

     void addCodeInt(unsigned int Code) {
        CodeInt.push_back(Code);
    }

    void addSymbolItem(string Name, unsigned int Offset) {
        shared_ptr<PoolItem> symbolItem = make_shared<PoolItem>(Name, Offset);
        this->SymbolItemsPool.push_back(symbolItem);
    }

    shared_ptr<PoolItem> getSymbolItem(string Name) {
        for (shared_ptr<PoolItem> symbolItem : SymbolItemsPool) {
            if (symbolItem->getName() == Name ) return symbolItem;
        }
        return {};
    }

    void addLiteralItem(string Name, unsigned int Offset) {
        shared_ptr<PoolItem> literalItem = make_shared<PoolItem>(Name, Offset);
        if(getLiteralItem(Name) == nullptr) {
            this->LiteralItemsPool.push_back(literalItem);
        }   // kreiraj tamo u asembleru pa ovde provera samo i push
    }

    shared_ptr<PoolItem> getLiteralItem(string Name) {
        for (shared_ptr<PoolItem> literalItem : LiteralItemsPool) {
            if (literalItem->getName() == Name ) return literalItem;
        }
        return {};
    }

    void addRelocationlItem(shared_ptr<RelocationItem> relocationItem ) {
        this->RelocationTableItems.push_back(relocationItem);
    }

    shared_ptr<RelocationItem> getRelocationItem(string Name) {
        for (shared_ptr<RelocationItem> relocationItem : RelocationTableItems) {
            if (relocationItem->getName() == Name ) return relocationItem;
        }
        return {};
    }

};

#endif