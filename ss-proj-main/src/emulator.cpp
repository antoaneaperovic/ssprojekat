#include "emulator.hpp"
#include "shared.hpp"
#include <fstream> 

void Emulator::emulate(){
    openFileAndReadData();
    prepareEmulatorData();
    processEmulatorData();
    printEmulatorData();
}

void Emulator::processEmulatorData() {
    for(int i = 0; i < emulatorInstructions.size(); i++) {
        string instructions = emulatorInstructions[i];
        while(instructions != "" && !haltInst) {
            // u redu ih imam dve ili jednu pa moramo da ih podelimo
            string tempInstruction = instructions.substr(0,11);
            cout << "TEMP INSTR: " << tempInstruction << endl;
            startInstructionCode(tempInstruction);
            if(instructions.length() > 11) instructions = instructions.substr(12);
            else instructions = "";
            //instructions = instructions.substr(12);
            //printEmulatorData();
        }
    }
}

void Emulator::startInstructionCode(string tempInstruction) {
    string instrCode = tempInstruction.substr(0,1);
    string mmmmCode = tempInstruction.substr(1,1);
    int aRegCode = hexToDec("0x" + tempInstruction.substr(3,1));
    int bRegCode = hexToDec("0x" + tempInstruction.substr(4,1));
    int cRegCode = hexToDec("0x" + tempInstruction.substr(6,1));
    string dRegCode = tempInstruction.substr(7,1) + tempInstruction.substr(9,2);

    cout << "INSTR:" << instrCode << "MMMM:" << mmmmCode << "rA:" << aRegCode << "rB:" << bRegCode << "rC:" << cRegCode << "rD:" << dRegCode << endl;

    string byte1 = instrCode + mmmmCode;
    if(byte1 == "00") {
        //halt tu su sve nule
        haltInst = true;
    }
    else if(byte1 == "10") {
        //int prva jedinica i posle nule, softv prekid
        //push status; push pc; cause<=4; status<=status&(~0x1); pc<=handle
        string data = decToHex(registersCSR[0], true);
        pushData(data);
        string dataPC = decToHex(registers[15], true);
        pushData(data);
        registersCSR[2] =4;
        registersCSR[0] = registersCSR[0]&(~0x4);
        //pc<=handler
        registers[15] = registersCSR[1];
    }
    else if(byte1 == "96" || byte1 == "95") {
        if(mmmmCode == "6") {
            //samo iret csr je rA, rA <= mem32[rB+rC+rD];
             uint32_t dataLocation = registers[bRegCode] + registers[cRegCode] + hexToDecEmulator(dRegCode);
            //zatim podatak sa adresa se smesta u registar csr[A]
            uint32_t dataFromMem = hexToDecEmulator(getDataFromMem(dataLocation));
            registersCSR[aRegCode] = dataFromMem;

        }
        else {
            //onda je 5
            registersCSR[aRegCode] = registersCSR[bRegCode] | hexToDecEmulator(dRegCode);
        }
    }
    else if(byte1 == "20" || byte1 == "21") {
        //call
        if(mmmmCode == "0") {
            //push pc; pc<=rA+rB+rD;
            uint32_t curPC = registers[15];
            //pushuje PC
            pushData(decToHex(curPC, true));
            //zatim u pc smestamo vrednost
            registers[15] = registers[aRegCode] + registers[bRegCode] + hexToDecEmulator(dRegCode);
            
        }
        else{
            // onda je 1 i tada imamo:
            //push pc; pc<=mem32[rA+rB+rD];
            uint32_t curPC = registers[15];
            //pushuje PC
            pushData(decToHex(curPC, true));
            //dohvatamo mem adresu
            uint32_t dataLocation = registers[aRegCode] + registers[bRegCode]  + hexToDecEmulator(dRegCode);
            //zatim podatak sa adresa
            cout << "MEM FROM DATA:" << memoryList[dataLocation] << endl;
            uint32_t dataFromMem = hexToDecEmulator(getDataFromMem(dataLocation));
            //smestamo u pc
            registers[15] = dataFromMem;
        }
    }
    else if(byte1 == "30" || byte1 == "38") {
        //jmp
        if(mmmmCode == "0") {
            // pc<=rA+rD;
            registers[15] = registers[aRegCode] + hexToDecEmulator(dRegCode);
        }
        else {
            // onda je 8 i tada je pc<=mem32[rA+rD];
            uint32_t dataLocation = registers[aRegCode] + hexToDecEmulator(dRegCode);
            //zatim podatak sa adresa
            uint32_t dataFromMem = hexToDecEmulator(getDataFromMem(dataLocation));
            registers[15] = dataFromMem;
        }
    }
    else if(byte1 == "31" || byte1 == "39") {
        //beq
        if(mmmmCode == "1") {
           if(registers[bRegCode] == registers[cRegCode]) registers[15] = registers[aRegCode] + hexToDecEmulator(dRegCode);
        }
        else {
            // onda je 9 i tada ako je uslov ispunjen rB==rC pc<=mem32[rA+rD];
            if(registers[bRegCode] == registers[cRegCode]) {
                uint32_t dataLocation = registers[aRegCode] + hexToDecEmulator(dRegCode);
                //zatim podatak sa adresa
                uint32_t dataFromMem = hexToDecEmulator(getDataFromMem(dataLocation));
                registers[15] = dataFromMem;
            }    
            
        }
    }
    else if(byte1 == "32" || byte1 == "3A") {
        //bne
        if(mmmmCode == "2") {
            // ako je uslov ispunjen rB!=rC pc<=rA+rD;
             if(registers[bRegCode] != registers[cRegCode]) registers[15] = registers[aRegCode] + hexToDecEmulator(dRegCode);
        }
        else {
            // onda je A i tada ako je uslov ispunjen rB!=rC pc<=mem32[rA+rD];
            if(registers[bRegCode] != registers[cRegCode]) {
                uint32_t dataLocation = registers[aRegCode] + hexToDecEmulator(dRegCode);
                //zatim podatak sa adresa
                uint32_t dataFromMem = hexToDecEmulator(getDataFromMem(dataLocation));
                registers[15] = dataFromMem;
            } 
            
        }
    }
    else if(byte1 == "33" || byte1 == "3B") {
        //bgt
        if(mmmmCode == "3") {
            // ako je uslov ispunjen rB signed>rC pc<=rA+rD;  
            // PAZI MOZDA MORA ODVOJENO DA SE DOHVATE I STAVE DA BUDU INT32_T
            if(registers[bRegCode] > registers[cRegCode]) registers[15] = registers[aRegCode] + hexToDecEmulator(dRegCode);
        }
        else {
            if(registers[bRegCode] > registers[cRegCode]) {
                uint32_t dataLocation = registers[aRegCode] + hexToDecEmulator(dRegCode);
                //zatim podatak sa adresa
                uint32_t dataFromMem = hexToDecEmulator(getDataFromMem(dataLocation));
                registers[15] = dataFromMem;
            } 
            
        }
    }
    else if(byte1 == "81") {
        //push rA<=rA+rD; mem32[rA] =rC
         registers[aRegCode] = registers[aRegCode] + hexToDecEmulator(dRegCode);
         uint32_t dataLocation = registers[aRegCode];
         int decNum = registers[cRegCode];
        string dataMem = decToHex(decNum, true);
        setDataInMem(dataLocation, dataMem);

    }
    else if(byte1 == "40") {
        //xchg rA je nula, rD je nula
        //temp <= rB; rB <= rC; rC <= temp;

        int temp = registers[bRegCode];
        registers[bRegCode] = registers[cRegCode];
        registers[cRegCode] = temp;

    }
    else if(byte1 == "50") {
        //add rA=rB+rC;
        registers[aRegCode] = registers[bRegCode] + registers[cRegCode];
        cout << "ADD";
    }
    else if(byte1 == "51") {
        //sub rA=rB-rC;
        registers[aRegCode] = registers[bRegCode] - registers[cRegCode];
          cout << "ADD";
    }
    else if(byte1 == "52") {
        //muv rA=rB*rC;
        registers[aRegCode] = registers[bRegCode] * registers[cRegCode];
          cout << "ADD";
    }
    else if(byte1 == "53") {
        //div rA=rB/rC;
        if(registers[cRegCode] != 0) registers[aRegCode] = registers[bRegCode] / registers[cRegCode];
    }
    else if(byte1 == "60") {
        //not rA = ~rB
        registers[aRegCode] = ~registers[bRegCode];
    }
    else if(byte1 == "61") {
        //and rA=rB&rC;
        registers[aRegCode] = registers[bRegCode] & registers[cRegCode];
    }
    else if(byte1 == "62") {
        //or rA=rB|rC;
        registers[aRegCode] = registers[bRegCode] | registers[cRegCode];
    }
    else if(byte1 == "63") {
        //xor rA=rB^rC;
        registers[aRegCode] = registers[bRegCode] ^ registers[cRegCode];
    }
    else if(byte1 == "70") {
        //shl rA = rB << rC
        registers[aRegCode] = registers[bRegCode] << registers[cRegCode];
    }
    else if(byte1 == "71") {
        //shr rA = rB >> rC
        registers[aRegCode] = registers[bRegCode] >> registers[cRegCode];
    }
    else if(byte1 == "91" || byte1 == "92") {
        //load
        if(mmmmCode == "1") {
            // rA <= rB + rD;
            registers[aRegCode] = registers[bRegCode] + hexToDecEmulator( dRegCode);
        }
        else {
            //onda 2 rA<=mem32[rB+rC+rD];
            uint32_t dataLocation = registers[bRegCode] + registers[cRegCode] + hexToDecEmulator(dRegCode);
            uint32_t dataFromMem = hexToDecEmulator(getDataFromMem(dataLocation));
            registers[aRegCode] = dataFromMem;
        }
    }
    else if(byte1 == "80" || byte1 == "82") {
        //store
        if(mmmmCode == "0") {
            //mem32[rA+rB+rD] <= rC;
            uint32_t dataLocation = registers[aRegCode] + registers[bRegCode] + hexToDecEmulator(dRegCode);
            int decNum = registers[cRegCode];
            string dataMem = decToHex(decNum, true);
            setDataInMem(dataLocation, dataMem);

        }
        else {
            //onda je 2 mem32[mem32[rA+rB+rD]] <= rC;
            //prvo izracunamo adresu
            uint32_t dataLocation = registers[aRegCode] + registers[bRegCode] + hexToDecEmulator(dRegCode);
            //zatim podatak sa adresa
            uint32_t dataLocationFromMem = hexToDecEmulator(getDataFromMem(dataLocation));
            //zatim kada imamo novu lokaciju na tu lokaciju smestamo podatak
            int decNum = registers[cRegCode];
            string dataMem = decToHex(decNum, true);
            setDataInMem(dataLocationFromMem, dataMem);
        }
    }
    else if(byte1 == "90") {
        //csrrd rA <= rB;  rB mi je csr
        registers[aRegCode] = registersCSR[bRegCode];
    }
    else if(byte1 == "94") {
        //csrwr rA <= rB; rA mi je csr 
        registersCSR[aRegCode] = registers[bRegCode];


    }
    else if(byte1 == "93") {
        //pop ili ret instrukcija
         // rA, rA <= mem32[rB]; rB <= rB + rD;
        uint32_t dataLocation = registers[bRegCode];
        //zatim podatak sa adresa se smesta u registar[A]
        uint32_t dataFromMem = hexToDecEmulator(getDataFromMem(dataLocation));
        registers[aRegCode] = dataFromMem;
        // nakon toga treba da update-ujemo registar[b] sa novom vrednoscu
        uint32_t newValue = registers[bRegCode] + hexToDecEmulator(dRegCode);
        registers[bRegCode] = newValue;
    }
    else if(byte1 == "97") {
        //csr[A]<=mem32[gpr[B]]; gpr[B]<=gpr[B]+D;
        uint32_t dataLocation = registers[bRegCode];
        //zatim podatak sa adresa se smesta u registar[A]
        uint32_t dataFromMem = hexToDecEmulator(getDataFromMem(dataLocation));
        registersCSR[aRegCode] = dataFromMem;
        // nakon toga treba da update-ujemo registar[b] sa novom vrednoscu
        uint32_t newValue = registers[bRegCode] + hexToDecEmulator(dRegCode);
        registers[bRegCode] = newValue;
    }
    else{
        //nista
    }

}


void Emulator::prepareEmulatorData(){
    for(int i = 0; i < 16; i++) registers[i] = 0;
    for(int i = 0; i < 3; i++) registersCSR[i] = 0;
    // u pitanju je SP
    //registers[14] = 0xFFFFFF02;
    //ovo je PC registar
    registers[15] = 0x40000000;
}

void Emulator::openFileAndReadData(){
    ifstream fileInput(fileInputName);

    if (!fileInput.is_open())
    {
        throw Exception("Greska pri otvaranju fajla");
    }
    string line;
    while (getline(fileInput, line)){ 
        int splitIndex = line.find(':');
        if(splitIndex != -1) {
            string addr = line.substr(0, splitIndex);
            string instructionCode = line.substr(splitIndex + 1);
            cout << "ADR: " << addr << " Ins" << instructionCode << endl;
            emulatorAddr.push_back(addr);
            uint32_t addrNum = hexToDecEmulator(addr);
            emulatorAddrInt.push_back(addrNum);
            emulatorInstructions.push_back(instructionCode);
            setInMemData(instructionCode.substr(0,11), addrNum);
            if(instructionCode.substr(12) != "") setInMemData(instructionCode.substr(12), addrNum + 4);
        }
      
    }

    fileInput.close();
}

void Emulator::setInMemData(string instruction, uint32_t addr) {
    int byte1 = stoll(instruction.substr(0,2), nullptr, 16); //"0x" + instruction.substr(0,2);
    int byte2 = stoll(instruction.substr(3,2), nullptr, 16); //"0x" + instruction.substr(3,2);
    int byte3 = stoll(instruction.substr(6,2), nullptr, 16); //"0x" + instruction.substr(6,2);
    int byte4 = stoll(instruction.substr(9,2), nullptr, 16); //"0x" + instruction.substr(9,2);
   
    memoryList[addr] = byte1;
    memoryList[addr + 1] = byte2;
    memoryList[addr + 2] = byte3;
    memoryList[addr + 3] = byte4;
    cout << "MEM addr:" << decToHex(addr,false) << "----> " ;
    cout << "MEM FROM DATA:" << decToHex(memoryList[addr], false) <<" ";
    cout << "" << decToHex(memoryList[addr+1] , false)<< " ";
    cout << "" << decToHex(memoryList[addr+2] , false)<< " ";
    cout << "" << decToHex(memoryList[addr+3], false) << endl;
}

void Emulator::pushData(string data){
    if(data == "0") data = "00000000";
    uint32_t newSP = registers[14] - 4;
    registers[14] = newSP;
    int j = 0;
    for(int i = 0; i < 4; i++) {
        if(memoryList.count(newSP+i) == 0) {
            memoryList[newSP + i] = 0;
        }
        memoryList[newSP + i] = stoll(data.substr(j,2), nullptr, 16);
        j += 2;
    }
}


string Emulator::getDataFromMem(uint32_t dataLocation)
{
    string data = "";
    cout << "MEM FROM DATA1:" << decToHex(memoryList[dataLocation], false) << endl;
    cout << "MEM FROM DATA2:" << decToHex(memoryList[dataLocation+1] , false)<< endl;
    cout << "MEM FROM DATA3:" << decToHex(memoryList[dataLocation+2] , false)<< endl;
    cout << "MEM FROM DATA4:" << decToHex(memoryList[dataLocation+3], false) << endl;
    for(int i = 0; i < 4; i++) {
        if(memoryList.count(dataLocation + i) == 0) {
            memoryList[dataLocation + i] = 0;
        }
        string hexNum = decToHex(memoryList[dataLocation + i], false);
        if(hexNum.length() == 1) hexNum = "0" + hexNum;
        data += hexNum;
        // data.push_back(decToHex(memoryList[curSP + i]))  -----> ako zelim sa stringovima
    }
    return data;
}

void Emulator::setDataInMem(uint32_t dataLocation, string data){
    if(data == "0") data = "00000000";
    int j = 0;
    for(int i = 0; i < 4; i++) {
        if(memoryList.count(dataLocation+i) == 0) {
            memoryList[dataLocation + i] = 0;
        }
        memoryList[dataLocation + i] = stoll(data.substr(j,2), nullptr, 16);
        j += 2;
    }
}

void Emulator::printEmulatorData(){
    //for(int i = 0; i < emulatorAddr.size(); i++) {
     //   cout << "Adresa: " << emulatorAddr[i] << "INT:" << emulatorAddrInt[i] << " --------- Instrukcija: " << emulatorInstructions[i] << endl;
   // }
    cout << "\n------------------------------------------------" << endl;
    cout << "Emulated processor executed halt instruction" << endl;
    cout << "Emulated processor state: " << endl;
    cout << "r0=" << hex << registers[0] << " r1=" << hex << registers[1] << " r2="
       << hex << registers[2] << " r3=" << hex << registers[3] << endl;
    cout << "r4=" << hex << registers[4] << " r5=" << hex << registers[5] << " r6="
        << hex << registers[6] << " r7=" << hex << registers[7] << endl;
    cout << "r8=" << hex << registers[8] << " r9=" << hex << registers[9] << " r10="
       << hex << registers[10] << " r11=" << hex << registers[11] << endl;
    cout << "r12=" << hex << registers[12] << " r13=" << hex << registers[13] << " r14="
       << hex << registers[14] << " r15=" << hex << registers[15] << endl;
}
