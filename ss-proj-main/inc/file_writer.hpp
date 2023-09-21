#ifndef FILE_WRITER
#define FILE_WRITER

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <memory>
#include "section.hpp"
#include "symbol.hpp"
#include "binary_record.hpp"

using namespace std;

class FileWriter
{
private:
    string outputFileName;

public:
    explicit FileWriter(const string& _outputFileName) : outputFileName(_outputFileName) {}
    virtual ~FileWriter() = default;

    virtual void writeSymbolsAndSections(vector<shared_ptr<Symbol>> symbols, vector<shared_ptr<Section>> sections);
    virtual void writeRecords(vector<shared_ptr<BinaryRecord>> records);
};

#endif /* FILE_WRITER */
