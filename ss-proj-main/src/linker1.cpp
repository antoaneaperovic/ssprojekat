#include "linker.hpp"

void Linker::execute()
{
    combineData();
    if (!isRelocatable)
    {
        calculateAndAddAddresses();
        resolveRelocations();
    }
}

void Linker::combineData()
{
    int symbolIndex = 0;
    for (int i = 0; i < sectionsFromAllFiles.size(); i++)
    {
        unordered_map<string, shared_ptr<Section>> &sectionsSmall = sectionsFromAllFiles[i];
        unordered_map<string, shared_ptr<Symbol>> &symbolsSmall = symbolsFromAllFiles[i];

        for (auto &sectionPair : sectionsSmall)
        {
            shared_ptr<Section> sectionSmall = sectionPair.second;

            if (mainSections.count(sectionSmall->getName()) == 0)
            {
                shared_ptr<Section> mainSection = make_shared<Section>(symbolIndex, sectionSmall->getName(), 0);
                mainSections[mainSection->getName()] = mainSection;
                shared_ptr<Symbol> mainSymbol = make_shared<Symbol>(symbolIndex, 0, sectionSmall->getName(), Symbol::Bind::LOCAL, Symbol::Type::SECTION, symbolIndex, sectionSmall->getName(), true);
                mainSymbols[mainSymbol->getName()] = mainSymbol;

                symbolIndex++;
            }

            shared_ptr<Section> section = mainSections[sectionSmall->getName()];

            int currentSize = section->getSize();

            section->incrementSize(sectionSmall->getSize());

            for (auto &byte : sectionSmall->getBytes())
            {
                section->addByte(byte);
            }

            for (shared_ptr<Section::RelocationEntry> &relocationSmall : sectionSmall->getRelocationTable())
            {
                section->addRelocationEntry(relocationSmall->getSymbolName(), currentSize + relocationSmall->getOffset(), relocationSmall->getAddend());
            }

            for (auto &symbolPair : symbolsSmall)
            {
                shared_ptr<Symbol> symbolSmall = symbolPair.second;

                // dodaju se samo simboli koji su u okviru sekcije koja se obradjuje, ali se zaobilaze symboli koji su takodje i sekcije.
                //
                // Samo ako je symbolSmall globalni se obradjuje, ako je extern, to znaci da ga uvozimo u fajl.
                // Da bi se symbolSmall nasao u mainSymbols, mora biti extern ili global.
                // Ako je symbolSmall extern, to znaci da se vec nalazi u mainSymbols i da je setovan na extern ili global.
                // Ako je symbolSmall global, i ako vec postoji u mainSymbol, to je greska, ako ne postoji ili je extern, onda se menjaju variable.
                if (symbolSmall->getSectionName() == section->getName() && symbolSmall->getType() != Symbol::Type::SECTION && symbolSmall->getBind() != Symbol::Bind::LOCAL)
                {
                    if (mainSymbols.count(symbolSmall->getName()) == 0)
                    {

                        shared_ptr<Symbol> mainSymbol = make_shared<Symbol>(symbolIndex, currentSize + symbolSmall->getValue(), symbolSmall->getName(), symbolSmall->getBind(), symbolSmall->getType(), section->getIndex(), section->getName(), symbolSmall->isDefined());
                        mainSymbols[mainSymbol->getName()] = mainSymbol;

                        symbolIndex++;
                    }
                    else
                    {
                        shared_ptr<Symbol> mainSymbol = mainSymbols[symbolSmall->getName()];

                        if (mainSymbol->getBind() == Symbol::Bind::GLOBAL && symbolSmall->getBind() == Symbol::Bind::GLOBAL)
                        {
                            throw Exception("Symbol \"" + mainSymbol->getName() + "\" is already defined as a global symbol.", 3);
                        }
                        else if (mainSymbol->getBind() == Symbol::Bind::EXTERN)
                        {
                            mainSymbol->setDefined(true);
                            mainSymbol->setNdx(section->getIndex());
                            mainSymbol->setValue(currentSize + symbolSmall->getValue());
                            mainSymbol->setType(symbolSmall->getType());
                            mainSymbol->setBind(Symbol::Bind::GLOBAL);
                            mainSymbol->setSectionName(section->getName());
                        }
                    }
                }
            }
        }
    }
}

void Linker::calculateAndAddAddresses()
{
    int highestOffset = 0;
    for (auto &sectionPair : mainSections)
    {
        shared_ptr<Section> section = sectionPair.second;

        if (places.count(section->getName()) == 1)
        {
            shared_ptr<Place> place = places[section->getName()];
            section->setOffset(place->getOffset());
            highestOffset = place->getOffset() + section->getSize();
        }
    }

    for (auto &sectionPair : mainSections)
    {
        shared_ptr<Section> section = sectionPair.second;
        if (places.count(section->getName()) == 0)
        {
            section->setOffset(highestOffset);
            highestOffset += section->getSize();
        }
    }

    for (auto &sectionPairCurrent : mainSections)
    {
        shared_ptr<Section> sectionCurrent = sectionPairCurrent.second;
        for (auto &sectionPairOther : mainSections)
        {
            shared_ptr<Section> sectionOther = sectionPairOther.second;

            if (sectionCurrent->getName() != sectionOther->getName())
            {
                if (sectionOther->getOffset() > sectionCurrent->getOffset() && sectionOther->getOffset() < sectionCurrent->getOffset() + sectionCurrent->getSize() ||
                    sectionCurrent->getOffset() > sectionOther->getOffset() && sectionCurrent->getOffset() < sectionOther->getOffset() + sectionOther->getSize())
                {
                    throw Exception("Section \"" + sectionCurrent->getName() + "\" (size: " + convert_int_to_hex(sectionCurrent->getSize()) + ", " + convert_int_to_hex(sectionCurrent->getOffset()) + " <-> " + convert_int_to_hex(sectionCurrent->getOffset() + sectionCurrent->getSize() - 1) + ") " +
                                        "and section \"" + sectionOther->getName() + "\" (size: " + convert_int_to_hex(sectionOther->getSize()) + ", " + convert_int_to_hex(sectionOther->getOffset()) + " <-> " + convert_int_to_hex(sectionOther->getOffset() + sectionOther->getSize() - 1) + ") overlap.",
                                    5);
                }
            }
        }
    }
}

void Linker::resolveRelocations()
{
    for (auto &sectionPair : mainSections)
    {
        shared_ptr<Section> section = sectionPair.second;
        vector<shared_ptr<Section::RelocationEntry>> relocations = section->getRelocationTable();

        for (shared_ptr<Section::RelocationEntry> &relocation : relocations)
        {
            shared_ptr<Symbol> symbol = mainSymbols[relocation->getSymbolName()];

            if (symbol->getSectionName() == "")
            {
                throw Exception("Symbol \"" + symbol->getName() + "\" is not defined.", 6);
            }

            shared_ptr<Section> sectionFromRelocation = mainSections[symbol->getSectionName()];
            unsigned int offset = relocation->getOffset();
            unsigned int value = sectionFromRelocation->getOffset() + symbol->getValue() + relocation->getAddend();

            vector<unsigned int> bytes = split_number_in_bytes(value, 4, true);

            for (int i = 0; i < bytes.size(); i++)
            {
                section->setByte(offset + i, bytes[i]);
            }
        }

        section->setRelocationTable({});
    }
}

vector<shared_ptr<Section>> Linker::getMainSections() const
{
    vector<shared_ptr<Section>> sections;
    for (auto &element : mainSections)
    {
        sections.push_back(element.second);
    }
    return sections;
}

vector<shared_ptr<Symbol>> Linker::getMainSymbols() const
{
    vector<shared_ptr<Symbol>> symbols;
    for (auto &element : mainSymbols)
    {
        symbols.push_back(element.second);
    }
    return symbols;
}

vector<shared_ptr<BinaryRecord>> Linker::generateRecords()
{
    vector<shared_ptr<BinaryRecord>> records;
    for (auto &element : mainSections)
    {
        shared_ptr<Section> section = element.second;

        // cout << element.first << " " << convert_int_to_hex_with_setw(section->getOffset(), 8) << " - " << convert_int_to_hex_with_setw(section->getOffset() + section->getSize() - 1, 8) << endl;

        unsigned int loop = section->getSize();
        unsigned int tmp = loop / 8;
        loop = loop - tmp * 8 == 0 <= tmp ? tmp : tmp + 1;
        vector<unsigned int> bytes = section->getBytes();

        if (bytes.empty())
        {
            continue;
        }

        for (int i = 0; i < loop; i++)
        {
            shared_ptr<BinaryRecord> binaryRecord = make_shared<BinaryRecord>(section->getOffset() + i * 8);

            for (int j = 0; j < 8; j++)
            {
                int index = i * 8 + j;
                if (bytes.size() <= index)
                {
                    break;
                }
                binaryRecord->addByte(bytes[index]);
            }
            if (!binaryRecord->getBytes().empty())
            {
                records.push_back(binaryRecord);
            }
        }
    }

    return records;
}
