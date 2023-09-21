#ifndef EMULATOR_H
#define EMULATOR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <iomanip>
#include <map>

#include "shared.hpp"
#include "exception.hpp"

using namespace std;

const uint32_t SP_START = 0xFFFFFF02;
const uint32_t PC_START = 0x40000000;

class Emulator
{
public:
    Emulator(string fileIN) {
        fileInputName = fileIN;
        emulatorAddr = {};
        emulatorInstructions = {};
    }
    virtual void emulate();


private:
    string fileInputName;
    //first je adresa, second je koda instrukcija na toj adresi
    vector<string> emulatorAddr;
    vector<uint32_t> emulatorAddrInt;
    vector<string> emulatorInstructions;
    //PC je 15, SP je 14;
    // array<uint32_t,16> registers;
    uint32_t registers[16];
    // status 0, handler 1, cause 2
    // array<uint32_t,3> registersCSR;
    uint32_t registersCSR[3];

    unordered_map<uint32_t,uint8_t> memoryList;

    bool haltInst = false;

    virtual void processEmulatorData();
    virtual void prepareEmulatorData();
    virtual void openFileAndReadData();
    virtual void printEmulatorData();
    virtual void startInstructionCode(string tempInstruction);
    virtual void setInMemData(string instruction, uint32_t addr);
    virtual void pushData(string data);
    virtual string getDataFromMem(uint32_t dataLocation);
    virtual void setDataInMem(uint32_t dataLocation, string data);

 
    //int sectionFileIndex;

};

#endif
