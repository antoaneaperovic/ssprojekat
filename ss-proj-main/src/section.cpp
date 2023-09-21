#include "section1.hpp"

    int Section::getID() {
	    return this->ID;
    }
    void setID(int ID) {
    	this->ID = ID;
    }

    unsigned getCounter() {
    	return this->Counter;
    }
    void setCounter(unsigned int Counter) {
    	this->Counter = Counter;
    }

    void incrementCounter(unsigned int Value) {
        this->Counter += Counter;
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
    void setSymbolItemsPool(vector<shared_ptr<PoolItem>> SymbolItemsPool) {
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

    unsigned getSize() {
    	return this->Size;
    }
    void setSize(unsigned int Size) {
    	this->Size = Size;
    }

    void incrementSize(unsigned int Value) {
        this->Size += Value;
    }

    vector<unsigned int> getCodeHex() {
	    return this->CodeHex;
    }
    void setCodeHex(int index, unsigned int CodeHex) {
    	this->CodeHex[index] = CodeHex;
    }

     void addCodeHex(unsigned int Code) {
        CodeHex.push_back(Code);
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
        shared_ptr<PoolItem> literalItem = make_shared<PoolItem>("", Offset);
        this->LiteralItemsPool.push_back(literalItem);
    }

    shared_ptr<PoolItem> getLiteralItem(unsigned int Value) {
        for (shared_ptr<PoolItem> literalItem : LiteralItemsPool) {
            if (literalItem->getValue() == Value ) return literalItem;
        }
        return {};
    }

    void addRelocationlItem(string Name, unsigned int Offset, unsigned int Addend) {
        shared_ptr<RelocationItem> relocationItem = make_shared<RelocationItem>(Name, Offset, Addend);
        this->RelocationTableItems.push_back(relocationItem);
    }

    shared_ptr<RelocationItem> getRelocationItem(string Name) {
        for (shared_ptr<RelocationItem> relocationItem : RelocationTableItems) {
            if (relocationItem->getName() == Name ) return relocationItem;
        }
        return {};
    }