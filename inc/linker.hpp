#ifndef LINKER_H
#define LINKER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <iomanip>
#include <map>

#include "section1.hpp"
#include "symbol1.hpp"
#include "place_item.hpp"
#include "shared.hpp"
#include "exception.hpp"

using namespace std;

const int START_MEM_REG_ADDR = 0xFFFFFF00;

class Linker
{
public:
    Linker(unordered_map<string, shared_ptr<PlaceItem>> pItemsList, vector<string> fsIN, string fOut, bool isH, int fiSize) {
        placeItemsList = pItemsList;
        fileInputNames = fsIN;
        fileOutputName = fOut;
        isHex = isH;
        fileInputSize = fiSize;
        linkerSymbolsCounter = 0;
        maxAddress = 0;
        //sectionFileIndex = 0;
    }
    virtual void link();


private:

    virtual void processLinkerData();

    virtual void changeOffsetForRelocationTable(string sectName, int sectionSize, shared_ptr<Section> asemSect);
    virtual void updateLinkerSymbols(string sectName, int sectionSize, int asemSectionSize);

    virtual void setNewStartingAddresses();
    virtual void setSectionsWithPlaceParam();
    virtual void setSectionsWithoutPlaceParam();
    virtual void checkOverlapping();
    virtual void checkAndSetHexCode();
    virtual void pushNewHexCode(shared_ptr<Section> section, string hexCode, int index);


    virtual void openFileAndReadDataBin();
    virtual void readSymbolsBin(ifstream& fileInputBin);
    virtual void readSectionsBin(ifstream& fileInputBin);

    virtual void addNewSymbolToSymbolsList(bool isLinker, int symbID, int value, string symbName, string symbType, int sectID, string sectName, bool isDef);
    virtual void addNewSectionToSectionsList(bool isLinker, int ID, string Name, int Size, vector<shared_ptr<RelocationItem>> relItems, vector<string> hexCode);
    
    virtual void openFileAndWriteData();
    virtual void printLinkerData(ofstream& fileOutput);
    virtual void printALLSymbols(ofstream& fileOutput);
    virtual void printALLSections(ofstream& fileOutput);
    

    unordered_map<string, shared_ptr<PlaceItem>> placeItemsList;
    vector<string> fileInputNames;
    string fileOutputName;
    bool isHex;
    int fileInputSize;
    //int sectionFileIndex;
    int linkerSymbolsCounter;
    int maxAddress;

    unordered_map<string, shared_ptr<Symbol>> symbolsLinker;
    unordered_map<string, shared_ptr<Section>> sectionsLinker;

    unordered_map<string, shared_ptr<Symbol>> symbolsAssembler;
    unordered_map<string, shared_ptr<Section>> sectionsAssembler;

};

#endif /* LINKER_H */
