#ifndef FILE_READER_H
#define FILE_READER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <utility>

#include "symbol1.hpp"
#include "section1.hpp"
#include "binary_record.hpp"

using namespace std;

class FileReader
{
private:
    string inputFileName;

public:
    explicit FileReader(const string &_inputFileName) : inputFileName(_inputFileName) {}
    virtual ~FileReader() = default;

    virtual vector<string> readLines();
    virtual pair<unordered_map<string, shared_ptr<Symbol>>, unordered_map<string, shared_ptr<Section>>> readSymbolsAndSections();
    virtual vector<shared_ptr<BinaryRecord>> readRecords();
};

#endif /* FILE_READER_H */
