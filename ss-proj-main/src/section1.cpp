#include "section.hpp"
#include "section1.hpp"
#include "../inc/section1.hpp"

int const Section::UNDEFINED = -1;

int Section::getIndex() const
{
    return index;
}

unsigned int Section::getLocationCounter() const
{
    return locationCounter;
}

string Section::getName() const
{
    return name;
}

void Section::incrementLocationCounter(const unsigned int inc)
{
    locationCounter += inc;
}

void Section::resetLocationCounter()
{
    locationCounter = 0;
}

void Section::addByte(const unsigned int byte)
{
    bytes.push_back(byte);
}

vector<unsigned int> Section::getBytes() const
{
    return bytes;
}

void Section::setByte(int position, unsigned int byte)
{
    bytes[position] = byte;
}

unsigned int Section::getOffset() const
{
    return offset;
}

void Section::setOffset(const unsigned int offset)
{
    this->offset = offset;
}

const string Section::toString()
{
    return (name.empty() ? "UND" : name) + "\t\t" + to_string(index) + "\t" + to_string(size);
}

void Section::addSymbolToPool(string symbol, unsigned int offset)
{
    shared_ptr<SectionSymbol> sectionSymbol = make_shared<SectionSymbol>(symbol, offset);
    symbolPool.push_back(sectionSymbol);
}

void Section::addLiteralToPool(unsigned int literal)
{
    shared_ptr<Section::SectionLiteral> sectionLiteral = getSectionLiteral(literal);
    if (sectionLiteral == nullptr)
    {
        literalPool.push_back(make_shared<SectionLiteral>(literal));
    }
}

void Section::addRelocationEntry(string symbolName, int offset, int addend)
{
    shared_ptr<RelocationEntry> relocationEntry = make_shared<RelocationEntry>(symbolName, offset, addend);
    relocationTable.push_back(relocationEntry);
}

shared_ptr<Section::SectionLiteral> Section::getSectionLiteral(unsigned int literal)
{
    for (shared_ptr<Section::SectionLiteral> sectionLiteral : literalPool)
    {
        if (sectionLiteral->getLiteral() == literal)
        {
            return sectionLiteral;
        }
    }

    return {};
}

shared_ptr<Section::SectionSymbol> Section::getSectionSymbol(string symbol)
{
    for (shared_ptr<Section::SectionSymbol> sectionSymbol : symbolPool)
    {
        if (sectionSymbol->getName() == symbol)
        {
            return sectionSymbol;
        }
    }

    return {};
}

vector<shared_ptr<Section::SectionSymbol>> Section::getSymbolPool() const
{
    return symbolPool;
}

vector<shared_ptr<Section::SectionLiteral>> Section::getLiteralPool() const
{
    return literalPool;
}

vector<shared_ptr<Section::RelocationEntry>> Section::getRelocationTable() const
{
    return relocationTable;
}

void Section::setSize(const unsigned int size)
{
    this->size = size;
}

unsigned int Section::getSize() const
{
    return size;
}

void Section::incrementSize(const unsigned int inc)
{
    size += inc;
}

void Section::clearSymbolPool()
{
    symbolPool.clear();
}

void Section::setRelocationTable(const vector<shared_ptr<RelocationEntry>> &relocationTable)
{
    this->relocationTable = relocationTable;
}
