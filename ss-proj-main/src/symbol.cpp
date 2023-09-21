#include "symbol.hpp"

void Symbol::increaseValue(unsigned int num)
{
    value += num;
}

int Symbol::getIndex() const
{
    return index;
}

unsigned int Symbol::getValue() const
{
    return value;
}

string Symbol::getName() const
{
    return name;
}

Symbol::Bind Symbol::getBind() const
{
    return bind;
}

Symbol::Type Symbol::getType() const
{
    return type;
}

int Symbol::getNdx() const
{
    return ndx;
}

bool Symbol::isDefined() const
{
    return defined;
}

string Symbol::getSectionName() const
{
    return sectionName;
}

void Symbol::setBind(Bind bind)
{
    this->bind = bind;
}

void Symbol::setDefined(bool defined)
{
    this->defined = defined;
}

void Symbol::setValue(unsigned int value)
{
    this->value = value;
}

void Symbol::setNdx(int ndx)
{
    this->ndx = ndx;
}

void Symbol::setType(Type type)
{
    this->type = type;
}

void Symbol::setSectionName(const string &sectionName)
{
    this->sectionName = sectionName;
}

const string Symbol::toString()
{
    string tt = "\t";
    if (name.size() < 8)
    {
        tt += "\t";
    }
    return (name.empty() ? "UND" : name) + tt + to_string(index) + "\t" + to_string(value) + "\t" + to_string(bind) + "\t" + to_string(type) + "\t" + to_string(ndx) + "\t" + to_string(defined);
}
