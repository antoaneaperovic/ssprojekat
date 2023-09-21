#include "assembler.hpp"
#include <fstream> 

void Assembler::execute()
{
    openFileAndReadData();
    generateFirstPass();
    generateSecondPass();
    openFileAndWriteData();
    openFileAndWriteDataBin();
}


void Assembler::addNewSymbolToCurrentSection(int symbID, int value, string symbName, string symbType, int sectID, string sectName, bool isDef) {
    shared_ptr<Symbol> tempSymb = make_shared<Symbol>(symbID, value, symbName, symbType, sectID, sectName, isDef);
    symbols[tempSymb->getName()] = tempSymb; 
}

void Assembler::openFileAndReadData()
{
    ifstream fileInput(fileInputName);

    if (!fileInput.is_open())
    {
        throw Exception("Input file \"" + fileInputName + "\" not exists.", 2);
    }
    string line;
    while (getline(fileInput, line))
    {
        lines.push_back(line);
    }

    fileInput.close();
}

void Assembler::generateFirstPass()
{
    //set dummies to avoid segmentation fault
    shared_ptr<Section> tempSection = make_shared<Section>(0, "UND");
    sections[tempSection->getName()] = tempSection;
    shared_ptr<Symbol> tempSymbol = make_shared<Symbol>(0, 0, "UND", "Local", sections["UND"]->getID(), sections["UND"]->getName(), true);
    symbols[tempSymbol->getName()] = tempSymbol;
    int lineNum = 1;
    int symbolID = 1;
    

    tempSection = sections["UND"];

    smatch matchingLine;
    string tempElemName = "";

    for(string line: lines) {
        line = clearSpaces(line);
        if(line.empty()) continue;
        else if(regex_search(line, matchingLine, DIRECTIVE_END)) {
            break;
        }
        else if(regex_search(line, matchingLine, DIRECTIVE_GLOBAL) || regex_search(line, matchingLine, DIRECTIVE_EXTERN)) {
            string firstParam = matchingLine.str(0);
            bool isGlobal = firstParam.at(1) == 'g';
            string type = isGlobal ? "Global" : "Extern"; 
             line = matchingLine.str(1);
            while(!line.empty()) {
                int index = line.find(',') == -1 ? line.length() : line.find(',');
                tempElemName = line.substr(0, index);
                if(symbols.count(tempElemName) != 0) {
                    //vec je generisan sad da vidimo jel okej
                    shared_ptr<Symbol> &tempSymb = symbols[tempElemName];
                    if(tempSymb->isSection()) throw Exception("Sekcija ne moze biti globalan/externi simbol. " + tempElemName, 1 , lineNum);
                    else if((tempSymb->getType() == "Extern" && isGlobal) || (!isGlobal && tempSymb->getType() == "Global")) {
                        throw Exception("Eksterni simbol ne moze biti definisan i kao globalan simbol. " + tempElemName, 2, lineNum);
                    }
                    tempSymb->setType(type);
                }
                else {
                    //ako prvi put naletimo na globalni simbol
                    addNewSymbolToCurrentSection(symbolID, 0, tempElemName, type, tempSection->getID(), tempSection->getName(), false);
                    symbolID++;
                }
                if(tempElemName == line) index = line.length() - 1;
                line = line.substr(index + 1);
            }
        }
        else if(regex_search(line, matchingLine, DIRECTIVE_SECTION)) {
            //cout << "LINE        :    sekcija     "  << line;
            tempElemName = matchingLine.str(1);
            //ako smo naisli na sekciju menjamo trenutnoj id i ime
            shared_ptr<Section> section = make_shared<Section>(symbolID, tempElemName);
            if(symbols.count(tempElemName) == 0) {
                //ako smo naisli na novu sekciju dodajemo je u sekcije
                sections[tempElemName] = section;
                //dodajemo je u tabelu simbola
                addNewSymbolToCurrentSection(symbolID, 0, tempElemName, "Local", symbolID, tempElemName, true);
                symbolID++;
            }
            else {
                //nije prvi put da je vidimo proveravamo sta se desava u tabeli simbola
                shared_ptr<Symbol> symb = symbols[tempElemName];
                if(!symb->isSection() && (symb->getType() != "Local" || symb->getisDefined())){
                    throw Exception("Sekcija je vec definisana. " + tempElemName, 3, lineNum);
                }
                if(symb->getisDefined()) {
                    tempSection = sections[tempElemName];
                }
                else {
                    sections[tempElemName] = section;
                    symb->setSectionName(tempElemName);
                    symb->setisDefined(true);
                }
            }
            //cout << "COUNTER" << tempSection->getCounter();
            // sections[tempSection->getName()]->setCounter(5);
            tempSection = sections[tempElemName];
        }
        else if(tempSection->getName() == "UND") { // ovo je svuda nadalje isto pa mozda bolje da se odma proverava
            throw Exception("Ne moze labela/instrukcija da bude pre sekcije" + tempElemName, 4, lineNum);
        }
        else if(regex_search(line, matchingLine, LABEL)) {
            tempElemName = matchingLine.str(1);
            if(symbols.count(tempElemName) == 0) {
                addNewSymbolToCurrentSection(symbolID, tempSection->getCounter(), tempElemName, "Local", tempSection->getID(), tempSection->getName(), true);
                symbolID++;
            }
            else {
                shared_ptr<Symbol> symb = symbols[tempElemName];
                if(symb->getisDefined()) throw Exception("Simbol " + tempElemName + " je vec definisan", 5, lineNum);
                if(symb->getType() == "Extern") throw Exception("Simbol " + tempElemName + " vec postoji kao eksterni simbol", 6, lineNum);
                //cout << "LABEL: sekcija: " << tempSection->getName() << tempSection->getID() << "sim: " << symb->getName() << endl;
                symb->setisDefined(true);
                if(symb->getType() != "Global") symb->setType("Local"); // nadjem je global u fajlu ali je definisan u istom pa ne stavljamo local
                symb->setValue(tempSection->getCounter());
                symb->setSectionID(tempSection->getID());
                symb->setSectionName(tempSection->getName());
            }      
        }
        else if(regex_search(line, matchingLine, DIRECTIVE_WORD)) {
            line = matchingLine.str(1);
            while(!line.empty()) {
                int index = line.find(',') == -1 ? line.length() : line.find(',');
                tempElemName = line.substr(0, index);
                tempSection->incrementCounter(4);
                if(tempElemName == line) index = line.length() - 1;
                line = line.substr(index + 1, line.length());
            }
        }
        else if(regex_search(line, matchingLine, DIRECTIVE_SKIP)) {
            line = matchingLine.str(1);
            int skipNum = line.find('x') != -1 ? stoi(line) : hexToDec(line.substr(2));
            tempSection->incrementCounter(skipNum);
        }
        else if(regex_search(line, matchingLine, INSTRUCTION_CALL) || regex_search(line, matchingLine, INSTRUCTION_JMP)) {
            tempElemName = matchingLine.str(2);
            string type = getOperandAddressingType(tempElemName);
            //cout << "CALL TYPE: " << type << " za " << tempElemName;
            if(type == "Mem_Symbol") {
                if(symbols.count(tempElemName) == 0) {
                    addNewSymbolToCurrentSection(symbolID, 0, tempElemName, "Local", sections["UND"]->getID(), sections["UND"]->getName(), false);
                    symbolID++;
                }
            }
            else {
                tempSection->addLiteralItem(to_string(hexToDec(tempElemName)), 0);
                //cout << "LITERAL ZA CALL DODAT" << hexToDec(tempElemName) <<  endl;
            }
            tempSection->incrementCounter(4);

        }
        else if(regex_search(line, matchingLine, INSTRUCTION_BEQ_BNE_BGT)) {
            tempElemName = matchingLine.str(8);
            string type = getOperandAddressingType(tempElemName);
            /*if(type != "Mem_Symbol" && type!= "Mem_Literal") {
                throw Exception("Lose napisana instrukcija skoka za: " + tempElemName, 8, lineNum);
            }*/
            if(type == "Mem_Symbol" && symbols.count(tempElemName) == 0) {
                //nije ok proveri 
                addNewSymbolToCurrentSection(symbolID, 0, tempElemName, "Local", sections["UND"]->getID(), sections["UND"]->getName(), false);
                symbolID++;
            }
            else {
                tempSection->addLiteralItem(to_string(hexToDec(tempElemName)), 0);
            }
            tempSection->incrementCounter(4);
        }
        else if(regex_search(line, matchingLine, INSTRUCTION_LOAD)) {
            tempElemName = matchingLine.str(2);
            string type = getOperandAddressingType(tempElemName);
            //cout << "LOAD TYPE:      "<< type << endl;
            if(type == "Literal") {
                int value = hexToDec(tempElemName);
                //if(value < -2048 || value > 2047) throw Exception("Ne moze literal koji je vec od 12 bita: " + tempElemName, 8, lineNum);
                tempSection->addLiteralItem(to_string(value), 0);
            }
            else if(type == "Mem_Literal") {
                tempSection->addLiteralItem(to_string(hexToDec(tempElemName)), 0);
                tempSection->incrementCounter(4); //posto se uzima iz memorije mora da se inkrementira za 4
            }
            else if(type == "Indirect_Register_Pom") {
                //op1 = matchingLine.str(15);
                //op2 = type == "Indirect_Register_Pom" ? matchingLine.str(18) : "0";
                tempElemName = matchingLine.str(18);
                  int value = hexToDec(tempElemName);
                  if(value < -2048 || value > 2047) throw Exception("Ne moze pomeraj koji je vec od 12 bita: " + tempElemName, 9, lineNum);   
            }
            else if(type == "Symbol" || type == "Mem_Symbol") {
                tempElemName = matchingLine.str(8);
                if(tempElemName.at(0) == '$') tempElemName = tempElemName.substr(1);
                if(symbols.count(tempElemName) == 0) {
                    addNewSymbolToCurrentSection(symbolID, 0, tempElemName, "Local", sections["UND"]->getID(), sections["UND"]->getName(), false);
                    symbolID++;
                }
                shared_ptr<Symbol> tempSymb = symbols[tempElemName];
                if(tempSymb->getType() == "Global" || tempSymb->getSectionID() != tempSection->getID()){
                    //u tom slucaju ako se radi o memorijskom adr moramo da dodamo simbolu za sekciju
                    if(type == "Mem_Symbol") {
                        tempSection->addSymbolItem(tempElemName, tempSection->getCounter());
                    }
                }
            }
            tempSection->incrementCounter(4); 
        }
        else if(regex_search(line, matchingLine, INSTRUCTION_STORE)) {
            tempElemName = matchingLine.str(5);
            string type = getOperandAddressingType(tempElemName);
            if(type == "Literal") {
                throw Exception("Ne moze literal da bude operand: " + tempElemName, 10, lineNum);
            }
            else if(type == "Mem_Literal") {
                tempSection->addLiteralItem(to_string(hexToDec(tempElemName)), 0);
            }
            else if(type == "Indirect_Register_Pom") {
                  tempElemName = matchingLine.str(21);
                  int value = hexToDec(tempElemName);
                  if(value < -2048 || value > 2047) throw Exception("Ne moze pomeraj koji je vec od 12 bita: " + tempElemName, 11, lineNum);   
            }
            else if(type == "Mem_Symbol") {
                if(symbols.count(tempElemName) == 0) {
                    addNewSymbolToCurrentSection(symbolID, 0, tempElemName, "Local", sections["UND"]->getID(), sections["UND"]->getName(), false);
                    symbolID++;
                }
            }
            else if(type == "Symbol") {
                throw Exception("Ne moze sao simbol da bude operand: " + tempElemName, 12, lineNum);
            }
            tempSection->incrementCounter(4); 
        }
        else if(regex_search(line, matchingLine, INSTRUCTION_HALT_INT_IRET_RET)) {
            tempElemName = matchingLine.str(1);
            int num = tempElemName != "iret" ? 4 : 8;
            tempSection->incrementCounter(num);
        }
        else {
            //za sve ostale: 
            //INSTRUCTION_CSRWR, INSTRUCTION_SHL_SHR, INSTRUCTION_XCHG, INSTRUCTION_NOT
            //INSTRUCTION_CSRRD, INSTRUCTION_AND_OR_XOR, INSTRUCTION_ADD_SUB_MUL_DIV, INSTRUCTION_PUSH_POP
            tempSection->incrementCounter(4);
        }

        lineNum++; 
         //cout << "COUNTER i sekcija" << tempSection->getCounter() << " " << tempSection->getName();
    }

    changeSymbolsOffset();
    changeLiteralsOffset();
    checkSymbolsDefinition();
    cout << "\n.....................PRVI PROLAZ...........................";
    //printALLSymbols(cout);
    //printALLSections(cout);
    cout << ".................KRAJ PRVOG PROLAZA........................\n\n\n";
}

void Assembler::generateSecondPass()
{
    //prvo da sve setujemo na null
    for(auto section: sections) {
        section.second->setCounter(0);
        section.second->setSymbolItemsPool();
    }

    int lineNum = 1;
    int symbolID = 1;

    shared_ptr<Section> tempSection = sections["UND"];
    tempSection->setCounter(0);

    smatch matchingLine;
    string instruction = "";

    for(string line: lines) {
        line = clearSpaces(line);
        //cout << line;
        if(line.empty()) continue;
        else if(regex_search(line, matchingLine, DIRECTIVE_END)) {
            break;
        }
        else if(regex_search(line, matchingLine, DIRECTIVE_GLOBAL) || regex_search(line, matchingLine, DIRECTIVE_EXTERN) || regex_search(line, matchingLine, LABEL)) {
            continue;
        }
        else if(regex_search(line, matchingLine, DIRECTIVE_SECTION)) {
            //cout << "LINE        :    Sekcija     "  << line << endl;
            instruction = matchingLine.str(1);
            tempSection = sections[instruction];
        }
        else if(regex_search(line,matchingLine, INSTRUCTION_CSRWR)) {
            instruction = matchingLine.str(1); 
            string op1 = matchingLine.str(2); // prvi elem
            string op2 = matchingLine.str(5); // drugi elem

            //treba napraviti sada kod za celu instrukciju (4bajta) 
            createInstructionCode(tempSection, instruction, op2, op1, "", 0, false);
        }
        else if(regex_search(line, matchingLine, INSTRUCTION_CSRRD)) {
            instruction = matchingLine.str(1); 
            string op1 = matchingLine.str(2); // prvi elem
            string op2 = matchingLine.str(5); // drugi elem

            //treba napraviti sada kod za celu instrukciju (4bajta) 
            createInstructionCode(tempSection, instruction, op2, op1, "", 0, false);
        }
        else if(regex_search(line, matchingLine, DIRECTIVE_WORD)) {
            line = matchingLine.str(1);
            //cout<< "WORD:   ";
            while(!line.empty()) {
                int index = line.find(',') == -1 ? line.length() : line.find(',');
                //cout << "INDEX:" << index <<"  " << line.length();
                instruction = line.substr(0, index);
                line = matchingLine.str(1);

                //cout << (isalpha(instruction.at(0)) != 0 ? "Symbol" : "Literal");

                if(isalpha(instruction.at(0)) != 0) {
                    //SIMBOL JE U PITANJU i tada dodajemo 0 bajtove u sekciju, ali i simbolu u relokacijonu tabelu
                    shared_ptr<Symbol> tempSymb = symbols[instruction];
                    for(int i = 0; i < 8; i++) tempSection->addCodeHex("0");
                    int value = tempSymb->getType() == "Extern" ? 0 : tempSymb->getValue();
                    shared_ptr<RelocationItem> relocationItem = make_shared<RelocationItem>(tempSymb->getName(), tempSection->getCounter(), value);
                    tempSection->addRelocationlItem(relocationItem);
                }
                else {
                    //LITERAL JE U PITANJU
                    //cout << "LITERAL " << line << endl;
                    int litNum = instruction.find('x') != -1 ? stoi(instruction) : hexToDec(instruction);
                    litNum = litNum == 0 ? 1 : litNum;
                    for(int i = 0; i < litNum*8; i++) {
                        tempSection->addCodeHex("0");
                    }    
                   // addLiteralHex(tempSection, decToHex(litNum));
                }

                tempSection->incrementCounter(4);

                //pomeramo se dalje
                if(instruction == line) index = line.length() - 1;
                line = line.substr(index + 1, line.length());
                
            }
        }
        else if(regex_search(line, matchingLine, DIRECTIVE_SKIP)) {
            line = matchingLine.str(1);
            int skipNum = line.find('x') != -1 ? stoi(line) : hexToDec(line.substr(2));
            for(int i=0; i < skipNum*2; i++) tempSection->addCodeHex("0");
            tempSection->incrementCounter(skipNum);
        }
        else if(regex_search(line, matchingLine, INSTRUCTION_CALL) || regex_search(line, matchingLine, INSTRUCTION_JMP)) {
            instruction = matchingLine.str(1);
            string op = matchingLine.str(2);
            string type = getOperandAddressingType(op);
            int regD = 0;
            bool isSecond = false;

            //cout << "CALL TYPE: " << type << " za " << op;

            if(type == "Mem_Symbol") {
                //AKO JE U PITANJU SIMBOL
                shared_ptr<Symbol> tempSymb = symbols[op];
                if(tempSymb->getType() == "Local" && tempSymb->getSectionID() == tempSection->getID()) {
                    //AKO JE LOKAL I U SEKCIJI JE ONDA SAMO KREIRAMO REGD
                    regD = tempSymb->getValue() - tempSection->getCounter() - 4;
                }
                else {
                    //STA AKO JE GLOBALNI SIMBOL ALI JE U SEKCIJI, DA LI CE BITI GRESKA AKO STAVIMO I ZA EXTERNE DA PROLAZI ???
                    //AKO NIJE U SEKCIJI
                    int value = 0;
                    isSecond = true;
                    if(tempSection->getSymbolItem(op) == nullptr) {
                        // OBRATI PAZNJU NA SLEDECU KOMANDU ZBOG SIZE -> ZA LINKER
                        tempSection->addSymbolItem(tempSymb->getName(), tempSection->getSize());
                        value = tempSection->getSize();
                        tempSection->incrementSize(4);
                    }
                    else {
                        value = tempSection->getSymbolItem(op)->getValue();
                    }
                    
                    regD = value - tempSection->getCounter() - 4;
                }
                
            }
            else {
                //AKO JE U PITANJU LITERAL
                isSecond = true;
                string litName = line.find('x') != -1 ? line : to_string(hexToDec(line));
                int value = tempSection->getLiteralItem(litName) == nullptr ? 0 : tempSection->getLiteralItem(litName)->getValue();
                regD = value - tempSection->getCounter() - 4;
            }
            createInstructionCode(tempSection, instruction, "pc", "", "", regD, isSecond);
        }
        else if(regex_search(line, matchingLine, INSTRUCTION_BEQ_BNE_BGT)) {
           instruction = matchingLine.str(1);
            string op1 = matchingLine.str(2);
            string op2 = matchingLine.str(5);
            string op3 = matchingLine.str(8);
            string type = getOperandAddressingType(op3);
            int regD = 0;
            bool isSecond = false;

            // << "BBB TYPE: " << type << " za " << op3;

            if(type == "Mem_Symbol") {
                //AKO JE U PITANJU SIMBOL
                shared_ptr<Symbol> tempSymb = symbols[op3];
                if(tempSymb->getType() == "Local" && tempSymb->getSectionID() == tempSection->getID()) {
                    //AKO JE LOKAL I U SEKCIJI JE ONDA SAMO KREIRAMO REGD
                    regD = tempSymb->getValue() - tempSection->getCounter() - 4;
                }
                else {
                    //STA AKO JE GLOBALNI SIMBOL ALI JE U SEKCIJI, DA LI CE BITI GRESKA AKO STAVIMO I ZA EXTERNE DA PROLAZI ???
                    //AKO NIJE U SEKCIJI
                    int value = 0;
                    isSecond = true;
                    if(tempSection->getSymbolItem(op3) == nullptr) {
                        // OBRATI PAZNJU NA SLEDECU KOMANDU ZBOG SIZE -> ZA LINKER
                        tempSection->addSymbolItem(tempSymb->getName(), tempSection->getSize());
                        value = tempSection->getSize();
                        tempSection->incrementSize(4);
                    }
                    else {
                        value = tempSection->getSymbolItem(op3)->getValue();
                    }
                    
                    regD = value - tempSection->getCounter() - 4;
                }
                
            }
            else {
                //AKO JE U PITANJU LITERAL
                isSecond = true;
                string litName = line.find('x') != -1 ? line : to_string(hexToDec(line));
                int value = tempSection->getLiteralItem(litName) == nullptr ? 0 : tempSection->getLiteralItem(litName)->getValue();
                regD = value - tempSection->getCounter() - 4;
            }
            createInstructionCode(tempSection, instruction, "pc", op1, op2, regD, isSecond);
        }
        else if(regex_search(line, matchingLine, INSTRUCTION_LOAD)) {
            instruction = matchingLine.str(1);
            string op1 = matchingLine.str(2);
            string op2 = matchingLine.str(24);
            int regD = 0;
            string regB = "";
            bool isSecond = false;

            string type = getOperandAddressingType(op1);
            //cout << "LOAD TYPE:      "<< type << endl;
            if(type == "Mem_Literal" || type == "Literal") {
                isSecond = true;
                shared_ptr<PoolItem> lit = tempSection->getLiteralItem(to_string(hexToDec(op1)));
                int value = lit->getValue() - tempSection->getCounter() - 4;
                regB = type == "Literal" ? "pc" : op2;
                regD = type == "Literal" ? value : 0;
                if(type == "Mem_Literal") {
                    createInstructionCode(tempSection, instruction, op1, "pc", "", value, isSecond);
                    //ako je regC prazan vratice nam nulu funcija za pravljenje maske
                    //tempSection->incrementCounter(4); 
                    //posto se uzima iz memorije mora da se inkrementira za 4
                }
                
            }
            else if(type == "Indirect_Register" || type == "Indirect_Register_Pom") {
                isSecond = true;
                op1 = matchingLine.str(15);
                string pom = type == "Indirect_Register_Pom" ? matchingLine.str(18) : "0";
                regB = op1;
                regD = hexToDec(pom);
            }
            else if(type == "Symbol" || type == "Mem_Symbol") {
                op1 = matchingLine.str(8);
                if(op1.at(0) == '$') op1 = op1.substr(1);
                shared_ptr<Symbol> tempSymb = symbols[op1];
                if(tempSymb->getType() == "Global" || tempSymb->getSectionID() != tempSection->getID()) {
                    regB = type == "Mem_Symbol" ? op2 : "pc";
                    int value = 0;
                    isSecond = true;
                    if(tempSection->getSymbolItem(op1) == nullptr) {
                        // OBRATI PAZNJU NA SLEDECU KOMANDU ZBOG SIZE -> ZA LINKER
                        tempSection->addSymbolItem(tempSymb->getName(), tempSection->getSize());
                        value = tempSection->getSize();
                        tempSection->incrementSize(4);
                    }
                    else {
                        value = tempSection->getSymbolItem(op1)->getValue();
                    }
                    
                    regD = value - tempSection->getCounter() - 4;
                    //u tom slucaju ako se radi o memorijskom adr moramo da dodamo hex code i povecamo counter
                    if(type == "Mem_Symbol") {
                        createInstructionCode(tempSection, instruction, op2, "pc", "", regD, isSecond);
                        regD = 0;
                    }
                }
                else {
                    isSecond = type == "Mem_Symbol";
                    regB = "pc";
                    regD = tempSymb->getValue() - tempSection->getCounter() - 4;
                }
            }
            else if(type == "Register") {
                regB = op1;
            }

            createInstructionCode(tempSection, instruction, op2, regB, "", regD, isSecond);
        }
        else if(regex_search(line, matchingLine, INSTRUCTION_STORE)) {
            instruction = matchingLine.str(1);
            string op1 = matchingLine.str(2);
            string op2 = matchingLine.str(5);
            int regD = 0;
            string regA = "";
            bool isSecond = false;
            string type = getOperandAddressingType(op2);
            if(type == "Mem_Literal") {
                isSecond = true;
                regA = "pc";
                op2 = matchingLine.str(6);
                shared_ptr<PoolItem> lit = tempSection->getLiteralItem(to_string(hexToDec(op2)));
                regD = lit->getValue() - tempSection->getCounter() - 4;
                
            }
            else if(type == "Indirect_Register_Pom" || type == "Indirect_Register") {
                regA = matchingLine.str(18);
                string value = matchingLine.str(21).empty() ? "0" : matchingLine.str(21);
                regD = hexToDec(value);
            }
            else if(type == "Mem_Symbol") {
                regA = "pc";
                //op1 = matchingLine.str(11);
                shared_ptr<Symbol> tempSymb = symbols[op2];
                if(tempSymb->getType() == "Global" || tempSymb->getSectionID() != tempSection->getID()) {
                    int value = 0;
                    isSecond = true;
                    if(tempSection->getSymbolItem(op2) == nullptr) {
                        // OBRATI PAZNJU NA SLEDECU KOMANDU ZBOG SIZE -> ZA LINKER
                        tempSection->addSymbolItem(tempSymb->getName(), tempSection->getSize());
                        value = tempSection->getSize();
                        tempSection->incrementSize(4);
                    }
                    else {
                        value = tempSection->getSymbolItem(op2)->getValue();
                    }
                    
                    regD = value - tempSection->getCounter() - 4;
                    //u tom slucaju ako se radi o memorijskom adr moramo da dodamo hex code i povecamo counter
                }
                else {
                    regA = "pc";
                    regD = tempSymb->getValue() - tempSection->getCounter() - 4;
                }
            }
            createInstructionCode(tempSection, instruction, regA, "", op1, regD, isSecond);
        }
        else if(regex_search(line, matchingLine, INSTRUCTION_HALT_INT_IRET_RET)) {
            instruction = matchingLine.str(1);
            if(instruction == "ret" || instruction == "halt" || instruction == "int") {
                bool isRet = instruction == "ret";
                int regD = isRet ? 4 : 0;
                string regB = isRet ? "sp" : "";
                string regA = isRet ? "pc": "";
                createInstructionCode(tempSection, instruction, regA, regB , "", regD, false);
            }
            else {
                //preostaje samo iret
                createInstructionCode(tempSection, instruction, "", "sp" , "", 4, false);
                createInstructionCode(tempSection, instruction, "pc", "sp" , "", 8, true);
            }
        }
        else if(regex_search(line, matchingLine, INSTRUCTION_NOT)) {
            instruction = matchingLine.str(1);
            string op1 = matchingLine.str(2);
            createInstructionCode(tempSection, instruction, op1, op1, "", 0, false);
            
        }
        else if(regex_search(line, matchingLine, INSTRUCTION_PUSH_POP)) {
            instruction = matchingLine.str(1);
            string op1 = matchingLine.str(2);
            string regA = instruction == "pop" ? op1 : "sp";
            string regB = instruction == "pop" ? "sp" : "";
            string regC = instruction == "pop" ? "" : op1;
            int regD = instruction == "pop" ? 4 : -4;
            createInstructionCode(tempSection, instruction, regA, regB, regC, regD, false);

        }
        else if(regex_search(line, matchingLine, INSTRUCTION_SHL_SHR) || regex_search(line, matchingLine, INSTRUCTION_XCHG) || regex_search(line, matchingLine, INSTRUCTION_AND_OR_XOR) || regex_search(line, matchingLine, INSTRUCTION_ADD_SUB_MUL_DIV)) {
            //za sve ostale: 
            //INSTRUCTION_SHL_SHR, INSTRUCTION_XCHG, 
            //INSTRUCTION_AND_OR_XOR, INSTRUCTION_ADD_SUB_MUL_DIV

            instruction = matchingLine.str(1);
            string op1 = matchingLine.str(2);
            string op2 = matchingLine.str(5);

            createInstructionCode(tempSection, instruction, op2, op2, op1, 0, false);

        }
        else { 
            throw Exception("Pogresna instrukcija");
        }
        //tempSection->incrementCounter(4);
        lineNum++;
    }

    cout << "\n.....................DRUGI PROLAZ...........................";
    putLiteralsToTheEnd();
    putSymbolsToTheEnd();
 
    
    cout << "...................KRAJ DRUGOG PROLAZA........................\n";

}


void Assembler::printALLSymbols(ostream& os)
{
    os <<"\n---------------------------Tabela simbola---------------------------\n";
    os <<"ID      |   NAME   |   TYPE   | VALUE  | SECTION | DEFINED |  SCOPE   \n";

    for(auto symb: symbols) {
       // printSymbol(cout, symb.second);
        os << symb.second;
    }
}

void Assembler::printALLSections(ostream& os)
{
    os <<"\n---------------------------Sekcije---------------------------\n";
    for(auto sect: sections) {
        os << sect.second;
    }
}

void Assembler::openFileAndWriteData()
{
    ofstream fileOUTPUT;
    fileOUTPUT.open(fileOutputName + ".txt");
    printALLSymbols(fileOUTPUT);
    printALLSections(fileOUTPUT);
    fileOUTPUT.close();
}

void Assembler::printALLSymbolsBin(ofstream& fileOutputBin){
    int size = symbols.size();
    fileOutputBin.write((const char *)&size, sizeof(int));
    for(auto symb: symbols){
        int id = symb.second->getID();
        fileOutputBin.write((const char *)&id, sizeof(int));
        int nameSize = symb.second->getName().size();
        fileOutputBin.write((const char *)&nameSize, sizeof(int));
        string name = symb.second->getName();
        fileOutputBin.write(name.c_str(), nameSize);
        unsigned int value = symb.second->getValue();
        fileOutputBin.write((const char *)&value, sizeof(unsigned int));

        int sectId = symb.second->getSectionID();
        fileOutputBin.write((const char *)&sectId, sizeof(int));
        int sectNameSize = symb.second->getSectionName().size();
        fileOutputBin.write((const char *)&sectNameSize, sizeof(int));
        string sectName = symb.second->getSectionName();
        fileOutputBin.write(sectName.c_str(), sectNameSize);

        int typeSize = symb.second->getType().size();
        fileOutputBin.write((const char *)&typeSize, sizeof(int));
        string type = symb.second->getType();
        fileOutputBin.write(type.c_str(), typeSize);

        bool isDef = symb.second->getisDefined();
        fileOutputBin.write((const char *)&isDef, sizeof(bool));

    }
}

void Assembler::printALLSectionsBin(ofstream& fileOutputBin){
    int sizeSect = sections.size();
    fileOutputBin.write((const char *)&sizeSect, sizeof(int));
    for(auto sect: sections){
        int sectId = sect.second->getID();
        fileOutputBin.write((const char *)&sectId, sizeof(int));
        int sectNameSize = sect.second->getName().size();
        fileOutputBin.write((const char *)&sectNameSize, sizeof(int));
        string sectName = sect.second->getName();
        fileOutputBin.write(sectName.c_str(), sectNameSize);

        auto const &relactionItems = sect.second->getRelocationTableItems();
        int rSize = relactionItems.size();
        fileOutputBin.write((const char *)&rSize, sizeof(int));

        for(auto rItem: relactionItems) {
            int rItemNameSize = rItem->getName().size();
            fileOutputBin.write((const char *)&rItemNameSize, sizeof(int));
            string rItemName = rItem->getName();
            fileOutputBin.write(rItemName.c_str(), rItemNameSize);
            unsigned int rItemOffset = rItem->getValue();
            fileOutputBin.write((const char *)&rItemOffset, sizeof(unsigned int));
            unsigned int rItemA = rItem->getAddend();
            fileOutputBin.write((const char *)&rItemA, sizeof(unsigned int));
        }

        unsigned int hexCodeSize = sect.second->getSize();
        fileOutputBin.write((const char *)&hexCodeSize, sizeof(unsigned int));

        for(string hexCode: sect.second->getCodeHex()) {
            int hexCodeSizeItem = hexCode.size();
            fileOutputBin.write((const char *)&hexCodeSizeItem, sizeof(int));
            fileOutputBin.write(hexCode.c_str(), hexCodeSizeItem);
        }

    }
}

void Assembler::openFileAndWriteDataBin()
{
    ofstream fileOutputBin(fileOutputName, ios::out | ios::binary);

    printALLSectionsBin(fileOutputBin);
    printALLSymbolsBin(fileOutputBin);
    fileOutputBin.close();
}

void Assembler::changeSymbolsOffset()
{
    for(auto sect: sections) {
        int countItems = 0;
        vector<shared_ptr<PoolItem>> sectionSymbolItems = sect.second->getSymbolItemsPool();
        for(auto symb: sectionSymbolItems) {
            shared_ptr<Symbol> symbFromTable = symbols[symb->getName()];
            if(symbFromTable->getType() == "Global" || symbFromTable->getSectionID() != sect.second->getID()) {
                sect.second->incrementCounter(4);
                for(auto symbTable: symbols) {
                    if(symbTable.second->getSectionID() == sect.second->getID() && symb->getValue() < symbTable.second->getValue()) {
                        symbTable.second->incrementValue(4);
                        int incFrom = countItems + 1;
                        while(incFrom < sectionSymbolItems.size()) {
                            sectionSymbolItems[incFrom++]->addToOffset(4);
                        }
                    }
                }
            }
            countItems++;  
        }
    }
}

void Assembler::changeLiteralsOffset(){
    for(auto sect: sections) {
        for(shared_ptr<PoolItem> lit: sect.second->getLiteralItemsPool()) {
            lit->setValue(sect.second->getCounter());
            sect.second->incrementCounter(4);
        }
        sect.second->setSize(sect.second->getCounter());
    }
}

void Assembler::checkSymbolsDefinition()
{
    //ostaje jos provera za sve simbole da li su definisani
    for(auto symb: symbols) {
        if(symb.second->getisDefined() || symb.second->getType() == "Extern") continue;
        throw Exception("Simbol nije definisan. Naziv je: " + symb.second->getName(), 13);
    } 
}

void Assembler::putLiteralsToTheEnd() {
    for(auto sect: sections) {
        for(shared_ptr<PoolItem> lit: sect.second->getLiteralItemsPool()) {
            int value = stoi(lit->getName());
            string value1 = decToHex(value, true);
            value1 = revertHex(value1);
            addLiteralHex(sect.second, value1);
        }
    }
}

void Assembler::putSymbolsToTheEnd() {
    for(auto sect: sections) {
        for(shared_ptr<PoolItem> symb: sect.second->getSymbolItemsPool()) {
            for(int i=0; i<8; i++) sect.second->addCodeHex("0");
            int value = 0;
            string name = symb->getName();
            if(symbols[symb->getName()] != nullptr && symbols[symb->getName()]->getType() == "Local") {
                value = symbols[symb->getName()]->getValue();
                name = symbols[symb->getName()]->getName();
            }
            shared_ptr<RelocationItem> relocationItem = make_shared<RelocationItem>(name, symb->getValue(), value);
            sect.second->addRelocationlItem(relocationItem);
            sect.second->incrementCounter(4);
        }
    }
}

void Assembler::addLiteralHex(shared_ptr<Section> tempSection, string hexNum)
{
    for(int i=0; i < hexNum.length(); i++) {
        tempSection->addCodeHex(hexNum.substr(i, 1));
    }
    tempSection->incrementCounter(4);
}

void Assembler::createInstructionCode(shared_ptr<Section> tempSection, string instruction, string op1, string op2, string c, int d, bool isSecond ){
    //        oc mod           regA regB          regC regD         regD regD
    string regC = getRegisterNum(c);
    string regD = decToHex(d,false).empty() ? "000" : decToHex(d,false);
    regD = regD.length() > 3 ? regD.substr(regD.length() - 3, 3) : regD;
    string regD1 = regD.length() == 3 ? regD.substr(0,1) : "0";
    string regD2 = regD.length() == 3 ? regD.substr(1,1) : (regD.length() == 2 ? regD.substr(0,1) : "0");
    string regD3 = regD.substr(regD.length() - 1, regD.length());
    //cout << "GENERISANJE KODA oc mod" << endl;
    tempSection->addCodeHex(getInstructionMask(instruction));
    tempSection->addCodeHex(getInstructionMaskMMMM(instruction, isSecond));

    //cout << "GENERISANJE KODA op1op2" << endl;
    tempSection->addCodeHex(getRegisterNum(op1));
    tempSection->addCodeHex(getRegisterNum(op2));

    //cout << "GENERISANJE KODA regCD" << endl;
    tempSection->addCodeHex(regC);
    tempSection->addCodeHex(regD1);

    // << "GENERISANJE KODA regDD" << endl;
    tempSection->addCodeHex(regD2);
    tempSection->addCodeHex(regD3);

    tempSection->incrementCounter(4);
}
