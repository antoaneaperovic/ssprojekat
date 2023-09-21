#include "file_writer.hpp"
#include "exception.hpp"
#include "utils.hpp"

void FileWriter::writeSymbolsAndSections(vector<shared_ptr<Symbol>> symbols, vector<shared_ptr<Section>> sections)
{
    ofstream outputFile(outputFileName, ios::binary);
    ofstream outputFileTxt(outputFileName + ".txt");

    int symbolsSize = symbols.size();
    outputFile.write((const char *)&symbolsSize, sizeof(int));
    for (auto &symbol : symbols)
    {
        int index = symbol->getIndex();
        outputFile.write((const char *)&index, sizeof(int));

        unsigned int value = symbol->getValue();
        outputFile.write((const char *)&value, sizeof(unsigned int));

        string name = symbol->getName();
        int nameLength = name.size();
        outputFile.write((const char *)&nameLength, sizeof(int));
        outputFile.write(name.c_str(), nameLength);

        string sectionName = symbol->getSectionName();
        int sectionNameLength = sectionName.size();
        outputFile.write((const char *)&sectionNameLength, sizeof(int));
        outputFile.write(sectionName.c_str(), sectionNameLength);

        Symbol::Bind bind = symbol->getBind();
        outputFile.write((const char *)&bind, sizeof(Symbol::Bind));

        Symbol::Type type = symbol->getType();
        outputFile.write((const char *)&type, sizeof(Symbol::Type));

        int ndx = symbol->getNdx();
        outputFile.write((const char *)&ndx, sizeof(int));

        bool defined = symbol->isDefined();
        outputFile.write((const char *)&defined, sizeof(bool));
    }

    int sectionsSize = sections.size();
    outputFile.write((const char *)&sectionsSize, sizeof(int));
    for (auto &section : sections)
    {
        int index = section->getIndex();
        string name = section->getName();
        int nameLength = name.size();
        outputFile.write((const char *)&index, sizeof(int));
        outputFile.write((const char *)&nameLength, sizeof(int));
        outputFile.write(name.c_str(), nameLength);

        auto const &relTable = section->getRelocationTable();
        int relSize = relTable.size();
        outputFile.write((const char *)&relSize, sizeof(int));
        for (auto const &relocation : relTable)
        {
            string symbolName = relocation->getSymbolName();
            int symbolNameLength = symbolName.size();
            outputFile.write((const char *)&symbolNameLength, sizeof(int));
            outputFile.write(symbolName.c_str(), symbolNameLength);

            unsigned int offset = relocation->getOffset();
            outputFile.write((const char *)&offset, sizeof(unsigned int));

            unsigned int addend = relocation->getAddend();
            outputFile.write((const char *)&addend, sizeof(unsigned int));
        }

        unsigned int size = section->getSize();
        outputFile.write((const char *)&size, sizeof(unsigned int));
        for (unsigned int byte : section->getBytes())
        {
            outputFile.write((const char *)&byte, sizeof(unsigned int));
        }
    }

    outputFile.flush();
    outputFile.close();

    string data;

    data += "#Sections:\n";

    for (auto &section : sections)
    {
        data += section->toString() + "\n";
        data += "#--literal pool:\n";
        for (auto &literal : section->getLiteralPool())
        {
            data += "\t\t" + to_string(literal->getLiteral()) + "\t" + to_string(literal->getOffset()) + "\n";
        }
        data += "#--sym pool:\n";
        for (auto &sym : section->getSymbolPool())
        {
            data += "\t\t" + sym->getName() + "\t" + to_string(sym->getOffset()) + "\n";
        }
        data += "#--rel table:\n";
        for (auto &relEntry : section->getRelocationTable())
        {
            data += "\t\t" + relEntry->getSymbolName() + "\t" + to_string(relEntry->getOffset()) + "\t" + to_string(relEntry->getAddend()) + "\n";
        }
        data += "#--bytes:\n";
        vector<unsigned int> bytes = section->getBytes();
        data += "\n#Data:\n";
        for (int i = 0; i < bytes.size() / 4; i++)
        {
            stringstream ss;
            for (int j = 0; j < 4; j++)
            {
                ss << convert_int_to_hex_with_setw(bytes[i * 4 + j], 2) << " ";
            }
            data += ss.str() + "\n";
        }
        data += "#--------\n";
    }
    data += "\n\n";

    data += "#symbolTable:\n";
    for (auto &symbol : symbols)
    {
        data += symbol->toString() + "\n";
    }
    data += "\n\n";

    // for (auto entry = symbolTable.begin(); entry != symbolTable.end(); entry++)
    // {
    //     data += entry->second->toString() + "\n";
    // }

    outputFileTxt << data;
    outputFile.flush();
    outputFile.close();
}

void FileWriter::writeRecords(vector<shared_ptr<BinaryRecord>> binaryRecords)
{
    ofstream outputFile(outputFileName, ios::binary);
    ofstream outputFileTxt(outputFileName + ".txt");

    int binaryRecordsSize = binaryRecords.size();
    outputFile.write((const char *)&binaryRecordsSize, sizeof(int));
    for (auto &record : binaryRecords)
    {
        unsigned int offset = record->getOffset();
        outputFile.write((const char *)&offset, sizeof(unsigned int));

        vector<unsigned int> bytes = record->getBytes();
        int bytesSize = bytes.size();
        outputFile.write((const char *)&bytesSize, sizeof(unsigned int));
        for (unsigned int byte : bytes)
        {
            outputFile.write((const char *)&byte, sizeof(unsigned int));
        }

        outputFileTxt << convert_int_to_hex_with_setw(record->getOffset(), 8) << ": ";
        for (auto &byte : record->getBytes())
        {
            outputFileTxt << convert_int_to_hex_with_setw(byte, 2) << " ";
        }
        outputFileTxt << endl;
    }

    outputFile.flush();
    outputFile.close();

    outputFileTxt.flush();
    outputFileTxt.close();
}
