#include "linker.hpp"

void Linker::link()
{
    openFileAndReadDataBin();
    processLinkerData();
    if(isHex) {
        setNewStartingAddresses();
    }
    else {
        //bilo bi -relocatable tacnije ni ne treba else
    }
    openFileAndWriteData();
    cout << "LINKED" << endl;
}

void Linker::processLinkerData(){

    for(auto asemSection: sectionsAssembler) {
        //cout << "ASEMBLER: " << asemSection.second->getName() << endl;
        string sectName = asemSection.second->getName();
        int asemSectionSize = asemSection.second->getSize();
        addNewSectionToSectionsList(true, linkerSymbolsCounter, sectName, 0, asemSection.second->getRelocationTableItems(), asemSection.second->getCodeHex());

        int linkerSectionSize = sectionsLinker[sectName]->getSize();
        shared_ptr<Section> tempSect = sectionsLinker[sectName];
        tempSect->incrementSize(asemSectionSize);
        //if(linkerSectionSize != 0) {
            changeOffsetForRelocationTable(sectName, linkerSectionSize, asemSection.second);
        //}
        updateLinkerSymbols(sectName, linkerSectionSize, asemSectionSize);  
    }
    
}

void Linker::changeOffsetForRelocationTable(string sectName, int linkerSectionSize, shared_ptr<Section> asemSect){
    shared_ptr<Section> tempSect = sectionsLinker[sectName];
    for(auto relocationItem: asemSect->getRelocationTableItems()) {
        int oldOffset = relocationItem->getValue();
        shared_ptr<RelocationItem> newItem = make_shared<RelocationItem>(relocationItem->getName(), oldOffset + linkerSectionSize, relocationItem->getAddend());
        tempSect->addRelocationlItem(newItem);
    }

    for(auto asemHexCode: asemSect->getCodeHex()) {
        tempSect->addCodeHex(asemHexCode);
    }
}

void Linker::updateLinkerSymbols(string sectName, int linkerSectionSize, int asemSectionSize){
    shared_ptr<Section> tempSect = sectionsLinker[sectName];
    //tempSect->incrementSize(asemSectionSize);

    for(auto asemSymbol: symbolsAssembler) {
        string symbName = asemSymbol.second->getName();
        int value = asemSymbol.second->getValue();
        string type = asemSymbol.second->getType();
        bool isDef = asemSymbol.second->getisDefined();
        string symbSectName = asemSymbol.second->getSectionName();

        if(symbName == symbSectName || symbSectName != sectName || type == "Local") {
            continue;
        }
        else {
            if(symbolsLinker.count(symbName) != 0) {
                shared_ptr<Symbol> tempSymb = symbolsLinker[symbName];
                if(tempSymb->getType() == "Extern") {
                    tempSymb->setType("Global");
                    tempSymb->setSectionName(tempSect->getName());
                    tempSymb->setisDefined(true);
                    tempSymb->setSectionID(tempSect->getID());
                    tempSymb->setValue(value + linkerSectionSize);
                }
                else {
                    // da li mora da se proveri da li su oba globalna i tempSymb i asemSymb
                    throw Exception("Simbol je definisan kao globalni");
                }
            }
            else {
                addNewSymbolToSymbolsList(true, linkerSymbolsCounter, value + linkerSectionSize, symbName, type, tempSect->getID(), sectName, isDef);
            }
        }
    }


}

void Linker::setNewStartingAddresses(){
    setSectionsWithPlaceParam();
    setSectionsWithoutPlaceParam();

    // sad nam preostaje da vidimo da li se neke preklapaju
    checkOverlapping();
    // ako je i to okej onda treba da setujemo hex code za svaku sekciju i relokacione tabele
    checkAndSetHexCode();

}

void Linker::setSectionsWithPlaceParam(){
    for(auto placeItem: placeItemsList) {
        for(auto linkerSection: sectionsLinker) {
            if(placeItem.second->getSection() == linkerSection.second->getName()) {
                int newAddr = placeItem.second->getSectionStartAddress();
                int sectSize = linkerSection.second->getSize();
                linkerSection.second->setHasPlaceAddress(true);
                linkerSection.second->setNewAddress(newAddr);
                if(newAddr + sectSize > START_MEM_REG_ADDR) {
                    //throw Exception("Sekcija zauzima vise mesta nego sto imamo u memoriji");
                }
                if(newAddr + sectSize > maxAddress) maxAddress = newAddr + sectSize;
            }
        }
    }
}

void Linker::setSectionsWithoutPlaceParam(){
    for(auto linkerSection: sectionsLinker) {
        if(!linkerSection.second->getHasPlaceAddress()) {
            int sectSize = linkerSection.second->getSize();
            linkerSection.second->setNewAddress(maxAddress);
            if(maxAddress + sectSize > START_MEM_REG_ADDR) {
                //throw Exception("Sekcija zauzima vise mesta nego sto imamo u memoriji.");
            }
            maxAddress += sectSize;
        }
    }
}

void Linker::checkOverlapping(){
    for(auto sectionFirst: sectionsLinker) {
        for(auto sectionSecond: sectionsLinker) {
            if(sectionFirst.second->getName() == sectionSecond.second->getName()) continue;
            int sizeFirst = sectionFirst.second->getSize() + sectionFirst.second->getNewAddress();
            int sizeSecond = sectionSecond.second->getSize() + sectionSecond.second->getNewAddress();

            int newAddrFirst = sectionFirst.second->getNewAddress();
            int newAddrSecond = sectionSecond.second->getNewAddress();

            if(newAddrFirst > newAddrSecond && newAddrFirst < sizeSecond) {
                throw Exception("Sekcije se preklapaju.");
            }
            if(newAddrSecond > newAddrFirst && newAddrSecond < sizeFirst) {
                throw Exception("Sekcije se preklapaju.");
            }
        }
    }
}

void Linker::checkAndSetHexCode(){
    for(auto linkerSection: sectionsLinker) {
        for(auto relocationItem: linkerSection.second->getRelocationTableItems()) {
            shared_ptr<Symbol> tempSymb = symbolsLinker[relocationItem->getName()];
            if(sectionsLinker[tempSymb->getSectionName()] != nullptr) {
                int index = relocationItem->getValue();
                int decCode = tempSymb->getValue() + sectionsLinker[tempSymb->getSectionName()]->getNewAddress() + relocationItem->getAddend();
                string hexCode = decToHex(decCode, false);
                hexCode = revertHex(hexCode);
                pushNewHexCode(linkerSection.second, hexCode, index);
            }
        }
    }
}

void Linker::pushNewHexCode(shared_ptr<Section> tempSection, string hexCode, int index){
    for(int i = 0; i < hexCode.length(); i++) {
        tempSection->setCodeHexWithIndex(index*2 + i, hexCode.substr(i, 1));
    }
}

void Linker::openFileAndReadDataBin(){
    for(int i = 0 ; i < fileInputSize; i++) {
        string fileName = fileInputNames[i];
        ifstream fileInputBin(fileName, ios::binary);
        cout<<"file name: " << fileName << endl;
        readSectionsBin(fileInputBin);
        readSymbolsBin(fileInputBin);
        fileInputBin.close();
        cout<<"file zatvoren name: " << fileName << endl;
    }
}

void Linker::readSymbolsBin(ifstream& fileInputBin){
    int size;
    fileInputBin.read((char *)&size, sizeof(int));

    for(int i = 0; i < size; i++){
        int id;
        fileInputBin.read((char *)&id, sizeof(int));

        int nameSize;
        fileInputBin.read((char *)&nameSize, sizeof(int));

        string name;
        name.resize(nameSize);
        fileInputBin.read((char *)name.c_str(), nameSize);

        unsigned int value;
        fileInputBin.read((char *)&value, sizeof(unsigned int));


        int sectId;
        fileInputBin.read((char *)&sectId, sizeof(int));

        int sectNameSize;
        fileInputBin.read((char *)&sectNameSize, sizeof(int));

        string sectName;
        sectName.resize(sectNameSize);
        fileInputBin.read((char *)sectName.c_str(), sectNameSize);

        int typeSize;
        fileInputBin.read((char *)&typeSize, sizeof(int));

        string type;
        type.resize(typeSize);
        fileInputBin.read((char *)type.c_str(), typeSize);

        bool isDef;
        fileInputBin.read((char *)&isDef, sizeof(bool));

        addNewSymbolToSymbolsList(false, id, value, name, type, sectId, sectName, isDef);
    }
}

void Linker::readSectionsBin(ifstream& fileInputBin){
    int sizeSect;
    fileInputBin.read((char *)&sizeSect, sizeof(int));
    for(int i = 0; i < sizeSect; i++){
        int sectId;
        fileInputBin.read((char *)&sectId, sizeof(int));

        int sectNameSize;
        fileInputBin.read((char *)&sectNameSize, sizeof(int));

        string sectName;
        sectName.resize(sectNameSize);
        fileInputBin.read((char *)sectName.c_str(), sectNameSize);

        vector<shared_ptr<RelocationItem>> relocationTableItems;
        vector<string> sectionHexCode;
        int rSize;
        fileInputBin.read((char *)&rSize, sizeof(int));

        for(int i1 = 0; i1 < rSize; i1++) {

            int rItemNameSize;
            fileInputBin.read((char *)&rItemNameSize, sizeof(int));

            string rItemName;
            rItemName.resize(rItemNameSize);
            fileInputBin.read((char *)rItemName.c_str(), rItemNameSize);

            unsigned int rItemOffset;
            fileInputBin.read((char *)&rItemOffset, sizeof(unsigned int));

            unsigned int rItemA;
            fileInputBin.read((char *)&rItemA, sizeof(unsigned int));

            shared_ptr<RelocationItem> relocationItem = make_shared<RelocationItem>(rItemName, rItemOffset, rItemA);
            relocationTableItems.push_back(relocationItem);
        }

        unsigned int hexCodeSize;
        fileInputBin.read((char *)&hexCodeSize, sizeof(unsigned int));

        for(int i2 = 0; i2 < hexCodeSize*2; i2++) {
            int hexCodeSizeItem;
            fileInputBin.read((char *)&hexCodeSizeItem, sizeof(int));

            string hexCode;
            hexCode.resize(hexCodeSizeItem);
            fileInputBin.read((char *)hexCode.c_str(), hexCodeSizeItem);

            sectionHexCode.push_back(hexCode);
        }

        addNewSectionToSectionsList(false, sectId, sectName, hexCodeSize, relocationTableItems, sectionHexCode);
    }
}

void Linker::addNewSymbolToSymbolsList(bool isLinker, int symbID, int value, string symbName, string symbType, int sectID, string sectName, bool isDef) {
    shared_ptr<Symbol> tempSymb = make_shared<Symbol>(symbID, value, symbName, symbType, sectID, sectName, isDef);
    if(isLinker) {
        symbolsLinker[tempSymb->getName()] = tempSymb;
        linkerSymbolsCounter++;
    }
    else {
        symbolsAssembler[tempSymb->getName()] = tempSymb;
    } 
}

void Linker::addNewSectionToSectionsList(bool isLinker, int ID, string Name, int Size, vector<shared_ptr<RelocationItem>> relItems, vector<string> hexCode) {
    shared_ptr<Section> tempSect = make_shared<Section>(ID, Name);
   
    if(isLinker) {
        if(sectionsLinker.count(Name) == 0) sectionsLinker[tempSect->getName()] = tempSect;
        if(symbolsLinker.count(Name) == 0) addNewSymbolToSymbolsList(true, linkerSymbolsCounter, 0, tempSect->getName(), "Local", tempSect->getID(), tempSect->getName(), true);
    }
    else {
        // dolazi iz fajla za asembler
        
        if(sectionsAssembler.count(Name) == 0) {
            sectionsAssembler[tempSect->getName()] = tempSect;
            tempSect->setRelocationTableItems(relItems);
            tempSect->setCodeHex(hexCode);
            tempSect->setSize(Size);
        }
        else {
            shared_ptr<Section> asemSect = sectionsAssembler[Name];
            // vodi racuna ovde mora i ako dolazi iz asemblera da se menja velicina za relokacione zapise
            int prevSize = asemSect->getSize();
            asemSect->incrementSize(Size);
            for(auto relocationItem: relItems) {
                int newValue = relocationItem->getValue() + prevSize;
                relocationItem->setValue(newValue);
                asemSect->addRelocationlItem(relocationItem);
            }
            for(auto asemHexCode: hexCode) {
                asemSect->addCodeHex(asemHexCode);
            }
        }
        
    }
}

void Linker::openFileAndWriteData(){
    ofstream fileOUTPUT;
    fileOUTPUT.open(fileOutputName + ".txt");
    printALLSymbols(fileOUTPUT);
    printALLSections(fileOUTPUT);
    fileOUTPUT.close();

    fileOUTPUT.open(fileOutputName);
    printLinkerData(fileOUTPUT);
    fileOUTPUT.close();
}

void Linker::printLinkerData(ofstream& fileOutput){
    for(auto sect2: sectionsLinker) {
        int addrCounter = 0;
        for (int i = 0; i < sect2.second->getCodeHex().size(); i+=2) {
                string item = sect2.second->getCodeHexWithIndex(i);
                char c = item.at(0);
                string item1 = sect2.second->getCodeHexWithIndex(i+1);
                char c1 = item1.at(0);
                string byte1 = item + item1;
                if (addrCounter % 8 == 0) fileOutput << hex << setw(8) << (sect2.second->getNewAddress() + addrCounter) << ":";
                fileOutput << hex << byte1 << " ";
                addrCounter++;
                if (addrCounter % 8 == 0) fileOutput << endl;
        }
        if(addrCounter%8 != 0)  fileOutput << endl;
    }
}

void Linker::printALLSymbols(ofstream& fileOutput){
    fileOutput <<"\n---------------------------Tabela simbola---------------------------\n";
    fileOutput <<"ID      |   NAME   |   TYPE   | VALUE  | SECTION | DEFINED |  SCOPE   \n";

    for(auto symb: symbolsAssembler) {
       // printSymbol(cout, symb.second);
        fileOutput << symb.second;
    }

    fileOutput <<"\n---------------------------LINKER---------------------------\n";
    for(auto symb1: symbolsLinker) {
        fileOutput << symb1.second;
    }
}

void Linker::printALLSections(ofstream& fileOutput){
    fileOutput <<"\n---------------------------Sekcije---------------------------\n";
    for(auto sect: sectionsAssembler) {
        fileOutput << sect.second;
    }

    fileOutput <<"\n---------------------------LINKER---------------------------\n";
    for(auto sect1: sectionsLinker) {
        fileOutput << sect1.second;
    }
    fileOutput << "\n---------------------------LINKER ADDRESS---------------------------\n";
    for(auto sect2: sectionsLinker) {
        int addrCounter = 0;
        fileOutput << "Section name: " << sect2.second->getName() << "\t Addr" << decToHex(sect2.second->getNewAddress(), false) << "\tHEXSIZE: " << sect2.second->getSize() << "\n";
        for (int i = 0; i < sect2.second->getCodeHex().size(); i+=2) {
                string item = sect2.second->getCodeHexWithIndex(i);
                char c = item.at(0);
                string item1 = sect2.second->getCodeHexWithIndex(i+1);
                char c1 = item1.at(0);
                if (addrCounter % 8 == 0) fileOutput << hex << setw(8) << (sect2.second->getNewAddress() + addrCounter) << "   ";
                fileOutput << hex << c << c1 << " ";
                addrCounter++;
                if (addrCounter % 8 == 0) fileOutput << endl;
        }
    }
}