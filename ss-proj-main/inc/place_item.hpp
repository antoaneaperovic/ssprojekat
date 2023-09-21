#ifndef PLACE_ITEM_H
#define PLACE_ITEM_H

#include <vector>
#include <memory>
#include<shared.hpp>

using namespace std;

class PlaceItem {
     private:
        string placeCommand;
        string sectionName;
        unsigned int sectionStartAddress;
    
    public:
    PlaceItem(string placeCom) {
        placeCommand = placeCom;
        setSectionNameAndStaringAddress(placeCom);

    }

    void setSectionNameAndStaringAddress(string command) {
        int i = command.find('=') + 1;
        int j = command.find('@');
        setSection(command.substr(i, j - i));
        int startAddr = hexToDec(command.substr(j + 1));
        setSectionStartAddress(startAddr);
    }

    string getPlaceCommand() {
        return placeCommand;
    }

    void setPlaceCommand(string command) {
        placeCommand = command;
    }

    string getSection() {
        return sectionName;
    }

    void setSection(string sectName) {
        sectionName = sectName;
    }

    unsigned int getSectionStartAddress() {
        return sectionStartAddress;
    }

    void setSectionStartAddress(unsigned int sectStartAddress) {
        sectionStartAddress = sectStartAddress;
    }

};

#endif