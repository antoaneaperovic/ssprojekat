#ifndef PLACE_H
#define PLACE_H

#include <iostream>
#include <string>
#include <regex>

#include "exception.hpp"
#include "utils.hpp"

using namespace std;

class Place
{
public:
    Place(string place);

    virtual string getSectionName() const
    {
        return sectionName;
    }

    virtual unsigned int getOffset() const
    {
        return offset;
    }

private:
    string sectionName;
    unsigned int offset;

    static regex REGEX_PLACE;
};

#endif /* PLACE_H */
