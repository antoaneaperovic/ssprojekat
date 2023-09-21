#ifndef LINKER_H
#define LINKER_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

#include "section.hpp"
#include "symbol.hpp"
#include "place.hpp"
#include "utils.hpp"
#include "exception.hpp"
#include "binary_record.hpp"

using namespace std;

class Linker
{
public:
    Linker(vector<unordered_map<string, shared_ptr<Symbol>>> _symbolsFromAllFiles, vector<unordered_map<string, shared_ptr<Section>>> _sectionsFromAllFiles, bool _isRelocatable, unordered_map<string, shared_ptr<Place>> _places)
        : symbolsFromAllFiles(_symbolsFromAllFiles), sectionsFromAllFiles(_sectionsFromAllFiles), isRelocatable(_isRelocatable), places(_places) {}

    virtual void execute();
    vector<shared_ptr<Symbol>> getMainSymbols() const;
    vector<shared_ptr<Section>> getMainSections() const;

    vector<shared_ptr<BinaryRecord>> generateRecords();

private:
    void combineData();
    void calculateAndAddAddresses();
    void resolveRelocations();

    vector<unordered_map<string, shared_ptr<Symbol>>> symbolsFromAllFiles;
    vector<unordered_map<string, shared_ptr<Section>>> sectionsFromAllFiles;
    unordered_map<string, shared_ptr<Place>> places;
    unordered_map<string, shared_ptr<Symbol>> mainSymbols;
    unordered_map<string, shared_ptr<Section>> mainSections;
    bool isRelocatable;
};

#endif /* LINKER_H */
