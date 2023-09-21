#ifndef SYMBOL_H
#define SYMBOL_H

#include <iostream>

using namespace std;

class Symbol
{
public:
    enum Bind
    {
        LOCAL = 0,
        GLOBAL,
        EXTERN,
    };
    enum Type
    {
        SECTION = 0,
        NO_TYPE,
    };

    Symbol(const int _index, const unsigned int _value, const string &_name, const Bind _bind, const Type _type, const int _ndx = 0, const string& _sectionName = "", const bool _defined = true)
        : index(_index), value(_value), name(_name), bind(_bind), type(_type), ndx(_ndx), sectionName(_sectionName), defined(_defined) {}

    void increaseValue(unsigned int num);
    int getIndex() const;
    unsigned int getValue() const;
    string getName() const;
    Bind getBind() const;
    Type getType() const;
    int getNdx() const;
    bool isDefined() const;
    string getSectionName() const;

    void setBind(Bind bind);
    void setDefined(bool defined);
    void setValue(unsigned int value);
    void setNdx(int ndx);
    void setType(Type type);
    void setSectionName(const string &sectionName);

    const string toString();

private:
    int index;
    unsigned int value;
    string name;
    string sectionName;
    Bind bind;
    Type type;
    int ndx;
    bool defined;
};

#endif /* SYMBOL_H */
