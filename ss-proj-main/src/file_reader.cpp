#include "file_reader.hpp"
#include "exception.hpp"

vector<string> FileReader::readLines()
{
    ifstream inputFile(inputFileName);

    if (!inputFile.is_open())
    {
        throw Exception("Input file \"" + inputFileName + "\" not exists.", 2);
    }
    string line;
    vector<string> lines;
    while (getline(inputFile, line))
    {
        lines.push_back(line);
    }

    inputFile.close();

    return lines;
}

pair<unordered_map<string, shared_ptr<Symbol>>, unordered_map<string, shared_ptr<Section>>> FileReader::readSymbolsAndSections()
{
   /*pair<unordered_map<string, shared_ptr<Symbol>>, unordered_map<string, shared_ptr<Section>>> symbolsAndSections;

    ifstream inputFile(inputFileName, ios::binary);

    if (inputFile.fail())
    {
        throw Exception("Input file \"" + inputFileName + "\" not exists.", 2);
    }

    unordered_map<string, shared_ptr<Symbol>> symbols;
    unordered_map<string, shared_ptr<Section>> sections;

    int symbolsSize;
    inputFile.read((char *)&symbolsSize, sizeof(int));

    for (int i = 0; i < symbolsSize; i++)
    {
        int index;
        inputFile.read((char *)&index, sizeof(int));

        unsigned int value;
        inputFile.read((char *)&value, sizeof(unsigned int));

        string name;
        int nameLength = name.size();
        inputFile.read((char *)&nameLength, sizeof(int));
        for (int j = 0; j < nameLength; j++)
        {
            char c;
            inputFile.read(&c, sizeof(char));
            name += c;
        }

        string sectionName;
        int sectionNameLength;
        inputFile.read((char *)&sectionNameLength, sizeof(int));
        for (int j = 0; j < sectionNameLength; j++)
        {
            char c;
            inputFile.read(&c, sizeof(char));
            sectionName += c;
        }

        Symbol::Bind bind;
        inputFile.read((char *)&bind, sizeof(Symbol::Bind));

        Symbol::Type type;
        inputFile.read((char *)&type, sizeof(Symbol::Type));

        int ndx;
        inputFile.read((char *)&ndx, sizeof(int));

        bool defined;
        inputFile.read((char *)&defined, sizeof(bool));

        shared_ptr<Symbol> symbol = make_shared<Symbol>(index, value, name, bind, type, ndx, sectionName, defined);
        symbols[symbol->getName()] = symbol;
    }

    int sectionsSize;
    inputFile.read((char *)&sectionsSize, sizeof(int));
    for (int i = 0; i < sectionsSize; i++)
    {
        int index;
        inputFile.read((char *)&index, sizeof(int));

        string name;
        int nameLength;
        inputFile.read((char *)&nameLength, sizeof(int));
        for (int j = 0; j < nameLength; j++)
        {
            char c;
            inputFile.read(&c, sizeof(char));
            name += c;
        }

        vector<shared_ptr<Section::RelocationEntry>> relocationEntries;
        int relSize;
        inputFile.read((char *)&relSize, sizeof(int));
        for (int j = 0; j < relSize; j++)
        {
            string symbolName;
            int symbolNameLength;
            inputFile.read((char *)&symbolNameLength, sizeof(int));
            for (int k = 0; k < symbolNameLength; k++)
            {
                char c;
                inputFile.read(&c, sizeof(char));
                symbolName += c;
            }

            unsigned int offset;
            inputFile.read((char *)&offset, sizeof(unsigned int));

            unsigned int addend;
            inputFile.read((char *)&addend, sizeof(unsigned int));
            shared_ptr<Section::RelocationEntry> relocationEntry = make_shared<Section::RelocationEntry>(symbolName, offset, addend);
            relocationEntries.push_back(relocationEntry);
        }

        unsigned int size;
        inputFile.read((char *)&size, sizeof(unsigned int));
        vector<unsigned int> bytes;
        for (int j = 0; j < size; j++)
        {
            unsigned int byte;
            inputFile.read((char *)&byte, sizeof(unsigned int));
            bytes.push_back(byte);
        }

        shared_ptr<Section> section = make_shared<Section>(index, name, size, relocationEntries, bytes);

        sections[section->getName()] = section;
    }

    inputFile.close();

    symbolsAndSections.first = symbols;
    symbolsAndSections.second = sections;

    return symbolsAndSections;*/ 
}

vector<shared_ptr<BinaryRecord>> FileReader::readRecords()
{
   /*ifstream inputFile(inputFileName, ios::binary);

    if (inputFile.fail())
    {
        throw Exception("Input file \"" + inputFileName + "\" not exists.", 2);
    }

    vector<shared_ptr<BinaryRecord>> records;

    int binaryRecordsSize;
    inputFile.read((char *)&binaryRecordsSize, sizeof(int));
    for (int i = 0; i < binaryRecordsSize; i++)
    {
        unsigned int offset;
        inputFile.read((char *)&offset, sizeof(unsigned int));

        int bytesSize;
        inputFile.read((char *)&bytesSize, sizeof(int));
        vector<unsigned int> bytes;
        for (int j = 0; j < bytesSize; j++)
        {
            unsigned int byte;
            inputFile.read((char *)&byte, sizeof(unsigned int));
            bytes.push_back(byte);
        }

        records.push_back(make_shared<BinaryRecord>(offset, bytes));
    }

    inputFile.close();

    return records;*/ 
}
