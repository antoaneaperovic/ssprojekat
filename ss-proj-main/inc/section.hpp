#ifndef SECTION_H
#define SECTION_H

#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Section
{
public:
    class SectionSymbol
    {
    public:
        SectionSymbol(string _name, unsigned int _offset) : name(_name), offset(_offset) {}
        void increaseOffset(unsigned int num)
        {
            offset += num;
        }

        string getName() const
        {
            return name;
        }

        unsigned int getOffset() const
        {
            return offset;
        }

    private:
        string name;
        unsigned int offset;
    };
    class SectionLiteral
    {
    public:
        SectionLiteral(unsigned int _literal, unsigned int _offset = 0) : literal(_literal), offset(_offset) {}

        void setOffset(unsigned int offset)
        {
            this->offset = offset;
        }

        unsigned int getLiteral() const
        {
            return literal;
        }

        unsigned int getOffset() const
        {
            return offset;
        }

    private:
        unsigned int literal;
        unsigned int offset;
    };

    class RelocationEntry
    {
    public:
        RelocationEntry(string _symbolName, int _offset, int _addend) : symbolName(_symbolName), offset(_offset), addend(_addend) {}
        string getSymbolName() const
        {
            return symbolName;
        }
        unsigned int getOffset() const
        {
            return offset;
        }
        unsigned int getAddend() const
        {
            return addend;
        }

    private:
        string symbolName;
        unsigned int offset;
        unsigned int addend;
    };

    Section(const int _index, const string &_name, const int _size = 0, vector<shared_ptr<RelocationEntry>> _relocationTable = {}, vector<unsigned int> _bytes = {})
        : index(_index), name(_name), size(_size), relocationTable(_relocationTable), bytes(_bytes) {}

    int getIndex() const;
    unsigned int getLocationCounter() const;
    string getName() const;

    void incrementLocationCounter(const unsigned int inc);
    void setSize(const unsigned int size);
    unsigned int getSize() const;
    void incrementSize(const unsigned int inc);
    void resetLocationCounter();
    void addByte(unsigned int byte);
    void setByte(int position, unsigned int byte);
    void addSymbolToPool(string symbol, unsigned int offset);
    void addRelocationEntry(string symbolName, int offset, int addend);
    void setRelocationTable(const vector<shared_ptr<RelocationEntry>> &relocationTable);
    vector<shared_ptr<SectionSymbol>> getSymbolPool() const;
    vector<shared_ptr<SectionLiteral>> getLiteralPool() const;
    vector<shared_ptr<RelocationEntry>> getRelocationTable() const;
    void clearSymbolPool();
    shared_ptr<SectionLiteral> getSectionLiteral(unsigned int literal);
    shared_ptr<Section::SectionSymbol> getSectionSymbol(string symbol);
    void addLiteralToPool(unsigned int literal);
    vector<unsigned int> getBytes() const;
    void setOffset(const unsigned int size);
    unsigned int getOffset() const;

    const string toString();

    const static int UNDEFINED;

private:
    int index;
    unsigned int locationCounter = 0;
    unsigned int offset = 0;
    string name;
    vector<shared_ptr<SectionSymbol>> symbolPool;
    vector<shared_ptr<SectionLiteral>> literalPool;
    vector<shared_ptr<RelocationEntry>> relocationTable;
    unsigned int size;
    vector<unsigned int> bytes;
};

#endif /* SECTION_H */
