#include "emulator.hpp"

Emulator::Emulator(vector<shared_ptr<BinaryRecord>> records)
{
    state = State::INIT;
    gpr = {
        make_pair(0, 0), // uvek je 0
        make_pair(1, 0),
        make_pair(2, 0),
        make_pair(3, 0),
        make_pair(4, 0),
        make_pair(5, 0),
        make_pair(6, 0),
        make_pair(7, 0),
        make_pair(8, 0),
        make_pair(9, 0),
        make_pair(10, 0),
        make_pair(11, 0),
        make_pair(12, 0),
        make_pair(13, 0),
        make_pair(14, START_STACK_ADDRESS), // sp
        make_pair(15, START_ADDRESS),       // pc
    };

    csr = {
        make_pair(0, -1), // status
        make_pair(1, 0),  // handler
        make_pair(2, 0),  // cause
    };

    updateMemoryLocation(TERM_IN, 0);

    terminalInterrupt = false;
    softwareInterrupt = false;
    badInstructionInterrupt = false;

    for (shared_ptr<BinaryRecord> &record : records)
    {
        int offset = record->getOffset();
        vector<unsigned int> bytes = record->getBytes();

        for (int i = 0; i < bytes.size(); i++)
        {
            memory[offset + i] = static_cast<uint8_t>(bytes[i]);
        }
    }
}

void Emulator::run()
{
    initTermios();
    thread termOut(&Emulator::threadTermOut, this);
    thread termIn(&Emulator::threadTermIn, this);

    cout << endl;
    cout << "Run" << endl;

    thread emulator(&Emulator::threadEmulator, this);

    emulator.join();
    termIn.join();
    termOut.join();
    resetTermios();
}

void Emulator::threadEmulator()
{
    while (state != State::END)
    {
        try
        {
            vector<uint8_t> bytes = getInstruction();
            InstructionParser instructionParser(bytes);
            processInstruction(instructionParser);
        }
        catch (exception &e)
        {
            cout << e.what() << endl;
            badInstructionInterrupt = true;
        }

        handleInterrupts();
    }
}

vector<uint8_t> Emulator::getInstruction()
{
    uint32_t pc = gpr[PC_POINTER];
    if (memory.count(pc) == 0)
    {
        throw runtime_error("Bad address; pc = " + convert_int_to_hex_with_setw(pc, 8));
    }
    vector<uint8_t> bytes;
    for (int i = 0; i < 4; i++)
    {
        bytes.push_back(memory[pc + i]);
    }

    gpr[15] = pc + 4;

    return bytes;
}
void Emulator::processInstruction(InstructionParser instructionParser)
{
    InstructionExecutor *instructionExecutor = InstructionExecutorFactory::getInstance()->getInstructionExecutor(instructionParser.getOc(), instructionParser.getMod());

    if (instructionExecutor == nullptr)
    {
        uint32_t pc = gpr[PC_POINTER] - 4;
        throw runtime_error("Bad instruction; pc = " + convert_int_to_hex_with_setw(pc, 8) + "; " + instructionParser.getData());
    }

    instructionExecutor->execute(*this, instructionParser.getRegA(), instructionParser.getRegB(), instructionParser.getRegC(), instructionParser.getDisp());
}

void Emulator::prepareForHandlngInterrupts()
{
    uint32_t statusData = getCSR(CSR_STATUS);
    push(statusData);
    uint32_t pcData = getGPR(PC_POINTER);
    push(pcData);
    updateCSR(CSR_STATUS, OFF, INTERRUPT_INDEX);
    uint32_t handleData = getCSR(CSR_HANDLER);
    updateGPR(PC_POINTER, handleData);
}

void Emulator::handleInterrupts()
{
    if (badInstructionInterrupt)
    {
        prepareForHandlngInterrupts();

        updateCSR(CSR_CAUSE, BAD_INSTRUCTION);
        badInstructionInterrupt = false;
    }
    else if (softwareInterrupt)
    {
        prepareForHandlngInterrupts();
        //instrukcija int

        updateCSR(CSR_CAUSE, SOFTWARE_INTERRUPT);
        softwareInterrupt = false;
    }
    else if (terminalInterrupt && getCSR(CSR_STATUS, TERMINAL_INDEX) == ON && getCSR(CSR_STATUS, INTERRUPT_INDEX) == ON)
    {
        prepareForHandlngInterrupts();

        updateCSR(CSR_CAUSE, TERMINAL_INTERRUPT);
        terminalInterrupt = false;
    }
}

void Emulator::updateCSR(int csr, uint32_t value, int index)
{
    if (index >= 0)
    {
        value = value != 0;
        uint32_t v = 1 << index;

        this->csr[csr] = (value == 1) * (this->csr[csr] | v) + (value == 0) * (this->csr[csr] & ~v);
    }
    else
    {
        this->csr[csr] = value;
    }
}

uint32_t Emulator::getCSR(int csr, int index)
{
    uint32_t value;
    if (index >= 0)
    {
        value = (this->csr[csr] & (1 << index)) >> index;
    }
    else
    {
        value = this->csr[csr];
    }

    return value;
}

void Emulator::updateGPR(int gpr, uint32_t value)
{
    if (gpr == 0)
    {
        throw runtime_error("Can't update registar r0!");
    }

    this->gpr[gpr] = value;
}

uint32_t Emulator::getGPR(int gpr)
{
    return this->gpr[gpr];
}

void Emulator::changeState(State state)
{
    this->state = state;
}

void Emulator::updateMemoryLocation(uint32_t location, uint32_t word)
{
    memoryMutex.lock();

    if (location == TERM_OUT)
    {
        termOutMemoryMappedRegister.push_front(word);
    }
    else
    {
        vector<uint8_t> bytes = em_split_number_in_bytes(word, true);

        for (int i = 0; i < 4; i++)
        {
            if (memory.count(location + i) == 0)
            {
                // cout << "Warning (Update): location (" + convert_int_to_hex_with_setw(location + i, 8) + ") is not in records." << endl;
                memory[location + i] = 0;
            }
            memory[location + i] = bytes[i];
        }
    }

    memoryMutex.unlock();
}

uint32_t Emulator::getMemoryLocation(uint32_t location)
{
    memoryMutex.lock();
    uint32_t word = 0;
    if (location == TERM_OUT)
    {
        if (!termOutMemoryMappedRegister.empty())
        {
            word = termOutMemoryMappedRegister.back();
            termOutMemoryMappedRegister.pop_back();
        }
    }
    else
    {
        vector<uint8_t> bytes;
        for (int i = 0; i < 4; i++)
        {
            if (memory.count(location + i) == 0)
            {
                // cout << "Warning (Get): location (" + convert_int_to_hex_with_setw(location + i, 8) + ") is not in records." << endl;
                memory[location + i] = 0;
            }
            bytes.push_back(memory[location + i]);
        }

        word = em_generate_data_from_bytes(bytes, true);
    }

    memoryMutex.unlock();
    return word;
}

void Emulator::push(uint32_t word)
{
    uint32_t spData = getGPR(STACK_POINTER) - 4;

    updateGPR(STACK_POINTER, spData);

    updateMemoryLocation(spData, word);
}

uint32_t Emulator::pop()
{
    uint32_t spData = getGPR(STACK_POINTER);

    uint32_t word = getMemoryLocation(spData);

    updateGPR(STACK_POINTER, spData + 4);

    return word;
}

void Emulator::callSoftwareInterrupt()
{
    softwareInterrupt = true;
}

void Emulator::threadTermOut()
{
    while (state != State::END)
    {
        uint32_t word = getMemoryLocation(TERM_OUT);
        char ch = word & 0xff;
        if (ch > 0)
        {
            cout << ch << flush;
        }
    }
}

void Emulator::threadTermIn()
{
    while (state != State::END)
    {
        char ch;
        if (read(STDIN_FILENO, &ch, 1) > 0)
        {
            updateMemoryLocation(TERM_IN, ch);
            terminalInterrupt = true;
        }
    }
}

void Emulator::initTermios()
{
    tcgetattr(0, &termOld);
    termNew = termOld;
    termNew.c_lflag &= ~(ECHO | ICANON);
    termNew.c_cc[VMIN] = 0;
    tcsetattr(0, TCSANOW, &termNew);
}

void Emulator::resetTermios()
{
    tcsetattr(0, TCSANOW, &termOld);
}
