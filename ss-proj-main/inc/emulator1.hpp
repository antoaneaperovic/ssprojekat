#ifndef EMULATOR_H
#define EMULATOR_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <deque>

#include "exception.hpp"
#include "binary_record.hpp"
#include "instruction_parser.hpp"
#include "instruction_executor_factory.hpp"
#include "instruction.hpp"
#include "utils.hpp"

using namespace std;

const unsigned long MEMORY_SIZE = 0x100000000;
const uint32_t START_ADDRESS = 0x40000000;
const uint32_t START_STACK_ADDRESS = 0xFFFFFF02; // 0xFFFFFF02 - 4 ce biti prvi put i to ce biti 0xFFFFFefe
const uint32_t TERM_IN = 0xFFFFFF04;
const uint32_t TERM_OUT = 0xFFFFFF00;

const int TERMINAL_INDEX = 1;
const int INTERRUPT_INDEX = 2;

const int ON = 1;
const int OFF = 0;

const int BAD_INSTRUCTION = 1;
const int TERMINAL_INTERRUPT = 3;
const int SOFTWARE_INTERRUPT = 4;

const int CSR_STATUS = 0;
const int CSR_HANDLER = 1;
const int CSR_CAUSE = 2;

const int STACK_POINTER = 14;
const int PC_POINTER = 15;

class Emulator
{
public:
    enum State
    {
        INIT = 0,
        RUNING,
        END
    };

    Emulator(vector<shared_ptr<BinaryRecord>> records);

    virtual void run();

    void updateGPR(int gpr, uint32_t value);
    uint32_t getGPR(int gpr);

    void updateCSR(int csr, uint32_t value, int index = -1);
    uint32_t getCSR(int csr, int index = -1);
    void changeState(State state);

    void updateMemoryLocation(uint32_t location, uint32_t word);
    uint32_t getMemoryLocation(uint32_t location);

    void push(uint32_t word);
    uint32_t pop();

    void callSoftwareInterrupt();

private:
    vector<uint8_t> getInstruction();
    void processInstruction(InstructionParser instructionParser);
    void handleInterrupts();
    void prepareForHandlngInterrupts();

    void initTermios();
    void resetTermios();
    void threadTermIn();
    void threadTermOut();

    void threadEmulator();

    unordered_map<uint32_t, uint8_t> memory;

    unordered_map<int, uint32_t> gpr;
    unordered_map<int, uint32_t> csr;

    State state;

    bool terminalInterrupt;
    bool softwareInterrupt;
    bool badInstructionInterrupt;

    struct termios termOld, termNew;

    mutex memoryMutex;

    deque<uint32_t> termOutMemoryMappedRegister;
};

#endif /* EMULATOR_H */
