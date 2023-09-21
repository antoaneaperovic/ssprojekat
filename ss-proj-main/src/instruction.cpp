#include "instruction.hpp"
#include "exception.hpp"

HaltInstructionExecutor *HaltInstructionExecutor::instance = new HaltInstructionExecutor();
void HaltInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    e.changeState(Emulator::State::END);
}

SoftwareInterruptInstructionExecutor *SoftwareInterruptInstructionExecutor::instance = new SoftwareInterruptInstructionExecutor();
void SoftwareInterruptInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    e.callSoftwareInterrupt();
}

CallInstructionExecutor *CallInstructionExecutor::instance = new CallInstructionExecutor();
void CallInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t pcData = e.getGPR(PC_POINTER);
    e.push(pcData);

    uint32_t gprAData = e.getGPR(regA);
    uint32_t gprBData = e.getGPR(regB);

    uint32_t newPcData = gprAData + gprBData + disp;

    e.updateGPR(PC_POINTER, newPcData);
}

CallWithMemInstructionExecutor *CallWithMemInstructionExecutor::instance = new CallWithMemInstructionExecutor();
void CallWithMemInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t pcData = e.getGPR(PC_POINTER);
    e.push(pcData);

    uint32_t gprAData = e.getGPR(regA);
    uint32_t gprBData = e.getGPR(regB);

    uint32_t location = gprAData + gprBData + disp;
    uint32_t newPcData = e.getMemoryLocation(location);
    e.updateGPR(PC_POINTER, newPcData);
}

JmpInstructionExecutor *JmpInstructionExecutor::instance = new JmpInstructionExecutor();
void JmpInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprAData = e.getGPR(regA);
    uint32_t newPcData = gprAData + disp;

    e.updateGPR(PC_POINTER, newPcData);
}

JmpBeqInstructionExecutor *JmpBeqInstructionExecutor::instance = new JmpBeqInstructionExecutor();
void JmpBeqInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprAData = e.getGPR(regA);
    uint32_t gprBData = e.getGPR(regB);
    uint32_t gprCData = e.getGPR(regC);

    if (gprBData == gprCData)
    {
        uint32_t newPcData = gprAData + disp;
        e.updateGPR(PC_POINTER, newPcData);
    }
}

JmpBneInstructionExecutor *JmpBneInstructionExecutor::instance = new JmpBneInstructionExecutor();
void JmpBneInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprAData = e.getGPR(regA);
    uint32_t gprBData = e.getGPR(regB);
    uint32_t gprCData = e.getGPR(regC);

    if (gprBData != gprCData)
    {
        uint32_t newPcData = gprAData + disp;
        e.updateGPR(PC_POINTER, newPcData);
    }
}

JmpBgtInstructionExecutor *JmpBgtInstructionExecutor::instance = new JmpBgtInstructionExecutor();
void JmpBgtInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprAData = e.getGPR(regA);
    uint32_t gprBData = e.getGPR(regB);
    uint32_t gprCData = e.getGPR(regC);

    if (((int32_t)gprBData) > ((int32_t)gprCData))
    {
        uint32_t newPcData = gprAData + disp;
        e.updateGPR(PC_POINTER, newPcData);
    }
}

JmpWithMemInstructionExecutor *JmpWithMemInstructionExecutor::instance = new JmpWithMemInstructionExecutor();
void JmpWithMemInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprAData = e.getGPR(regA);
    uint32_t newPcData = gprAData + disp;

    uint32_t location = gprAData + disp;
    uint32_t word = e.getMemoryLocation(location);
    e.updateGPR(PC_POINTER, word);
}

JmpBeqWithMemInstructionExecutor *JmpBeqWithMemInstructionExecutor::instance = new JmpBeqWithMemInstructionExecutor();
void JmpBeqWithMemInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprAData = e.getGPR(regA);
    uint32_t gprBData = e.getGPR(regB);
    uint32_t gprCData = e.getGPR(regC);

    if (gprBData == gprCData)
    {
        uint32_t location = gprAData + disp;
        uint32_t newPcData = e.getMemoryLocation(location);
        e.updateGPR(PC_POINTER, newPcData);
    }
}

JmpBneWithMemInstructionExecutor *JmpBneWithMemInstructionExecutor::instance = new JmpBneWithMemInstructionExecutor();
void JmpBneWithMemInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprAData = e.getGPR(regA);
    uint32_t gprBData = e.getGPR(regB);
    uint32_t gprCData = e.getGPR(regC);

    if (gprBData != gprCData)
    {
        uint32_t location = gprAData + disp;
        uint32_t newPcData = e.getMemoryLocation(location);
        e.updateGPR(PC_POINTER, newPcData);
    }
}

JmpBgtWithMemInstructionExecutor *JmpBgtWithMemInstructionExecutor::instance = new JmpBgtWithMemInstructionExecutor();
void JmpBgtWithMemInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprAData = e.getGPR(regA);
    uint32_t gprBData = e.getGPR(regB);
    uint32_t gprCData = e.getGPR(regC);

    if (((int32_t)gprBData) > ((int32_t)gprCData))
    {
        uint32_t location = gprAData + disp;
        uint32_t newPcData = e.getMemoryLocation(location);
        e.updateGPR(PC_POINTER, newPcData);
    }
}

XchgMemInstructionExecutor *XchgMemInstructionExecutor::instance = new XchgMemInstructionExecutor();
void XchgMemInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    int32_t gprBData = e.getGPR(regB);
    int32_t gprCData = e.getGPR(regC);

    e.updateGPR(regB, gprCData);
    e.updateGPR(regC, gprBData);
}

// ARITMETIC
AddInstructionExecutor *AddInstructionExecutor::instance = new AddInstructionExecutor();
void AddInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    int32_t gprBData = e.getGPR(regB);
    int32_t gprCData = e.getGPR(regC);

    e.updateGPR(regA, gprBData + gprCData);
}

SubInstructionExecutor *SubInstructionExecutor::instance = new SubInstructionExecutor();
void SubInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    int32_t gprBData = e.getGPR(regB);
    int32_t gprCData = e.getGPR(regC);

    e.updateGPR(regA, gprBData - gprCData);
}

MulInstructionExecutor *MulInstructionExecutor::instance = new MulInstructionExecutor();
void MulInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    int32_t gprBData = e.getGPR(regB);
    int32_t gprCData = e.getGPR(regC);

    e.updateGPR(regA, gprBData * gprCData);
}

DivInstructionExecutor *DivInstructionExecutor::instance = new DivInstructionExecutor();
void DivInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    int32_t gprBData = e.getGPR(regB);
    int32_t gprCData = e.getGPR(regC);

    e.updateGPR(regA, gprBData / gprCData);
}

// LOGIC
NotInstructionExecutor *NotInstructionExecutor::instance = new NotInstructionExecutor();
void NotInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprBData = e.getGPR(regB);

    e.updateGPR(regA, ~gprBData);
}

AndInstructionExecutor *AndInstructionExecutor::instance = new AndInstructionExecutor();
void AndInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprBData = e.getGPR(regB);
    uint32_t gprCData = e.getGPR(regC);

    e.updateGPR(regA, gprBData & gprCData);
}

OrInstructionExecutor *OrInstructionExecutor::instance = new OrInstructionExecutor();
void OrInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprBData = e.getGPR(regB);
    uint32_t gprCData = e.getGPR(regC);

    e.updateGPR(regA, gprBData | gprCData);
}

XorInstructionExecutor *XorInstructionExecutor::instance = new XorInstructionExecutor();
void XorInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprBData = e.getGPR(regB);
    uint32_t gprCData = e.getGPR(regC);

    e.updateGPR(regA, gprBData ^ gprCData);
}

// SHIFT
ShlInstructionExecutor *ShlInstructionExecutor::instance = new ShlInstructionExecutor();
void ShlInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    int32_t gprBData = e.getGPR(regB);
    int32_t gprCData = e.getGPR(regC);

    e.updateGPR(regA, gprBData << gprCData);
}

ShrInstructionExecutor *ShrInstructionExecutor::instance = new ShrInstructionExecutor();
void ShrInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    int32_t gprBData = e.getGPR(regB);
    int32_t gprCData = e.getGPR(regC);

    e.updateGPR(regA, gprBData >> gprCData);
}

// STORE
StoreInstructionExecutor *StoreInstructionExecutor::instance = new StoreInstructionExecutor();
void StoreInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprAData = e.getGPR(regA);
    uint32_t gprBData = e.getGPR(regB);
    uint32_t gprCData = e.getGPR(regC);

    uint32_t location = gprAData + gprBData + disp;

    e.updateMemoryLocation(location, gprCData);
}

StoreMemInstructionExecutor *StoreMemInstructionExecutor::instance = new StoreMemInstructionExecutor();
void StoreMemInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprAData = e.getGPR(regA);
    uint32_t gprBData = e.getGPR(regB);
    uint32_t gprCData = e.getGPR(regC);

    uint32_t location = gprAData + gprBData + disp;

    uint32_t locationFromLocation = e.getMemoryLocation(location);

    e.updateMemoryLocation(locationFromLocation, gprCData);
}

StoreWithUpdateInstructionExecutor *StoreWithUpdateInstructionExecutor::instance = new StoreWithUpdateInstructionExecutor();
void StoreWithUpdateInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprAData = e.getGPR(regA);
    uint32_t gprCData = e.getGPR(regC);

    uint32_t newRegAData = gprAData + disp;
    e.updateGPR(regA, newRegAData);

    uint32_t location = newRegAData;
    e.updateMemoryLocation(location, gprCData);
}

// LOAD
LoadGprFromCsrInstructionExecutor *LoadGprFromCsrInstructionExecutor::instance = new LoadGprFromCsrInstructionExecutor();
void LoadGprFromCsrInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t csrBData = e.getCSR(regB);
    e.updateGPR(regA, csrBData);
}

LoadGprFromGprInstructionExecutor *LoadGprFromGprInstructionExecutor::instance = new LoadGprFromGprInstructionExecutor();
void LoadGprFromGprInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprBData = e.getGPR(regB);
    uint32_t newGprBData = gprBData + disp;
    e.updateGPR(regA, newGprBData);
}

LoadGprFromMemInstructionExecutor *LoadGprFromMemInstructionExecutor::instance = new LoadGprFromMemInstructionExecutor();
void LoadGprFromMemInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprBData = e.getGPR(regB);
    uint32_t gprCData = e.getGPR(regC);

    uint32_t location = gprBData + gprCData + disp;

    uint32_t word = e.getMemoryLocation(location);
    e.updateGPR(regA, word);
}

LoadPopInstructionExecutor *LoadPopInstructionExecutor::instance = new LoadPopInstructionExecutor();
void LoadPopInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprBData = e.getGPR(regB);
    uint32_t word = e.getMemoryLocation(gprBData);
    e.updateGPR(regA, word);

    uint32_t newRegBData = gprBData + disp;
    e.updateGPR(regB, newRegBData);
}

LoadCsrFromGprInstructionExecutor *LoadCsrFromGprInstructionExecutor::instance = new LoadCsrFromGprInstructionExecutor();
void LoadCsrFromGprInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprBData = e.getGPR(regB);
    e.updateCSR(regA, gprBData);
}

LoadCsrOrDispInstructionExecutor *LoadCsrOrDispInstructionExecutor::instance = new LoadCsrOrDispInstructionExecutor();
void LoadCsrOrDispInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t csrBData = e.getCSR(regB);
    uint32_t newCsrBData = csrBData | disp;
    e.updateCSR(regA, newCsrBData);
}

LoadCsrFromMemInstructionExecutor *LoadCsrFromMemInstructionExecutor::instance = new LoadCsrFromMemInstructionExecutor();
void LoadCsrFromMemInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprBData = e.getGPR(regB);
    uint32_t gprCData = e.getGPR(regC);

    uint32_t location = gprBData + gprCData + disp;
    uint32_t word = e.getMemoryLocation(location);
    e.updateCSR(regA, word);
}

LoadPopCsrInstructionExecutor *LoadPopCsrInstructionExecutor::instance = new LoadPopCsrInstructionExecutor();
void LoadPopCsrInstructionExecutor::execute(Emulator &e, int regA, int regB, int regC, int disp)
{
    uint32_t gprBData = e.getGPR(regB);
    uint32_t word = e.getMemoryLocation(gprBData);
    e.updateCSR(regA, word);

    uint32_t newRegBData = gprBData + disp;
    e.updateGPR(regB, newRegBData);
}
