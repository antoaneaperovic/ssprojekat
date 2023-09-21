#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <emulator.hpp>
#include "instruction_executor_factory.hpp"

using namespace std;

class Emulator;
//-------------------------------------------InstructionExecutor----------------------------------------------//
class InstructionExecutor
{
protected:
    virtual int getOC() = 0;
    virtual int getMOD() = 0;

public:
    InstructionExecutor() {}
    virtual ~InstructionExecutor() {}
    virtual void execute(Emulator &e, int regA, int regB, int regC, int disp) = 0;

protected:
    void addExecutorToFactory()
    {
        InstructionExecutorFactory *factory = InstructionExecutorFactory::getInstance();
        factory->addInstructionExecutor(this, getOC(), getMOD());
    }
};

//-------------------------------------------HaltInstructionExecutor----------------------------------------------//
class HaltInstructionExecutor : public InstructionExecutor
{
public:
    HaltInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0000;
    }
    virtual int getMOD()
    {
        return 0b0000;
    }

private:
    static HaltInstructionExecutor *instance;
};

//-------------------------------------------SoftwareInterruptInstructionExecutor----------------------------------------------//
class SoftwareInterruptInstructionExecutor : public InstructionExecutor
{
public:
    SoftwareInterruptInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0001;
    }
    virtual int getMOD()
    {
        return 0b0000;
    }

private:
    static SoftwareInterruptInstructionExecutor *instance;
};

//-------------------------------------------CallInstructionExecutor----------------------------------------------//
class CallInstructionExecutor : public InstructionExecutor
{
public:
    CallInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0010;
    }
    virtual int getMOD()
    {
        return 0b0000;
    }

private:
    static CallInstructionExecutor *instance;
};

//-------------------------------------------CallWithMemInstructionExecutor----------------------------------------------//
class CallWithMemInstructionExecutor : public InstructionExecutor
{
public:
    CallWithMemInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0010;
    }
    virtual int getMOD()
    {
        return 0b0001;
    }

private:
    static CallWithMemInstructionExecutor *instance;
};

//-------------------------------------------JmpInstructionExecutor----------------------------------------------//
class JmpInstructionExecutor : public InstructionExecutor
{
public:
    JmpInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0011;
    }
    virtual int getMOD()
    {
        return 0b0000;
    }

private:
    static JmpInstructionExecutor *instance;
};

//-------------------------------------------JmpBeqInstructionExecutor----------------------------------------------//
class JmpBeqInstructionExecutor : public InstructionExecutor
{
public:
    JmpBeqInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0011;
    }
    virtual int getMOD()
    {
        return 0b0001;
    }

private:
    static JmpBeqInstructionExecutor *instance;
};

//-------------------------------------------JmpBneInstructionExecutor----------------------------------------------//
class JmpBneInstructionExecutor : public InstructionExecutor
{
public:
    JmpBneInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0011;
    }
    virtual int getMOD()
    {
        return 0b0010;
    }

private:
    static JmpBneInstructionExecutor *instance;
};

//-------------------------------------------JmpBgtInstructionExecutor----------------------------------------------//
class JmpBgtInstructionExecutor : public InstructionExecutor
{
public:
    JmpBgtInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0011;
    }
    virtual int getMOD()
    {
        return 0b0011;
    }

private:
    static JmpBgtInstructionExecutor *instance;
};

//-------------------------------------------JmpWithMemInstructionExecutor----------------------------------------------//
class JmpWithMemInstructionExecutor : public InstructionExecutor
{
public:
    JmpWithMemInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0011;
    }
    virtual int getMOD()
    {
        return 0b1000;
    }

private:
    static JmpWithMemInstructionExecutor *instance;
};

//-------------------------------------------JmpBeqWithMemInstructionExecutor----------------------------------------------//
class JmpBeqWithMemInstructionExecutor : public InstructionExecutor
{
public:
    JmpBeqWithMemInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0011;
    }
    virtual int getMOD()
    {
        return 0b1001;
    }

private:
    static JmpBeqWithMemInstructionExecutor *instance;
};

//-------------------------------------------JmpBneWithMemInstructionExecutor----------------------------------------------//
class JmpBneWithMemInstructionExecutor : public InstructionExecutor
{
public:
    JmpBneWithMemInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0011;
    }
    virtual int getMOD()
    {
        return 0b1010;
    }

private:
    static JmpBneWithMemInstructionExecutor *instance;
};

//-------------------------------------------JmpBgtWithMemInstructionExecutor----------------------------------------------//
class JmpBgtWithMemInstructionExecutor : public InstructionExecutor
{
public:
    JmpBgtWithMemInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0011;
    }
    virtual int getMOD()
    {
        return 0b1011;
    }

private:
    static JmpBgtWithMemInstructionExecutor *instance;
};

//-------------------------------------------XchgMemInstructionExecutor----------------------------------------------//
class XchgMemInstructionExecutor : public InstructionExecutor
{
public:
    XchgMemInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0100;
    }
    virtual int getMOD()
    {
        return 0b0000;
    }

private:
    static XchgMemInstructionExecutor *instance;
};

//-------------------------------------------AddInstructionExecutor----------------------------------------------//
class AddInstructionExecutor : public InstructionExecutor
{
public:
    AddInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0101;
    }
    virtual int getMOD()
    {
        return 0b0000;
    }

private:
    static AddInstructionExecutor *instance;
};

//-------------------------------------------SubInstructionExecutor----------------------------------------------//
class SubInstructionExecutor : public InstructionExecutor
{
public:
    SubInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0101;
    }
    virtual int getMOD()
    {
        return 0b0001;
    }

private:
    static SubInstructionExecutor *instance;
};

//-------------------------------------------MulInstructionExecutor----------------------------------------------//
class MulInstructionExecutor : public InstructionExecutor
{
public:
    MulInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0101;
    }
    virtual int getMOD()
    {
        return 0b0010;
    }

private:
    static MulInstructionExecutor *instance;
};

//-------------------------------------------DivInstructionExecutor----------------------------------------------//
class DivInstructionExecutor : public InstructionExecutor
{
public:
    DivInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0101;
    }
    virtual int getMOD()
    {
        return 0b0011;
    }

private:
    static DivInstructionExecutor *instance;
};

//-------------------------------------------NotInstructionExecutor----------------------------------------------//
class NotInstructionExecutor : public InstructionExecutor
{
public:
    NotInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0110;
    }
    virtual int getMOD()
    {
        return 0b0000;
    }

private:
    static NotInstructionExecutor *instance;
};

//-------------------------------------------AndInstructionExecutor----------------------------------------------//
class AndInstructionExecutor : public InstructionExecutor
{
public:
    AndInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0110;
    }
    virtual int getMOD()
    {
        return 0b0001;
    }

private:
    static AndInstructionExecutor *instance;
};


//-------------------------------------------OrInstructionExecutor----------------------------------------------//
class OrInstructionExecutor : public InstructionExecutor
{
public:
    OrInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0110;
    }
    virtual int getMOD()
    {
        return 0b0010;
    }

private:
    static OrInstructionExecutor *instance;
};

//-------------------------------------------XorInstructionExecutor----------------------------------------------//
class XorInstructionExecutor : public InstructionExecutor
{
public:
    XorInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0110;
    }
    virtual int getMOD()
    {
        return 0b0011;
    }

private:
    static XorInstructionExecutor *instance;
};

//-------------------------------------------ShlInstructionExecutor----------------------------------------------//
class ShlInstructionExecutor : public InstructionExecutor
{
public:
    ShlInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0111;
    }
    virtual int getMOD()
    {
        return 0b0000;
    }

private:
    static ShlInstructionExecutor *instance;
};

//-------------------------------------------ShrInstructionExecutor----------------------------------------------//
class ShrInstructionExecutor : public InstructionExecutor
{
public:
    ShrInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b0111;
    }
    virtual int getMOD()
    {
        return 0b0001;
    }

private:
    static ShrInstructionExecutor *instance;
};

//-------------------------------------------StoreMemInstructionExecutor----------------------------------------------//
class StoreInstructionExecutor : public InstructionExecutor
{
public:
    StoreInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b1000;
    }
    virtual int getMOD()
    {
        return 0b0000;
    }

private:
    static StoreInstructionExecutor *instance;
};

//-------------------------------------------StoreMemMemInstructionExecutor----------------------------------------------//
class StoreMemInstructionExecutor : public InstructionExecutor
{
public:
    StoreMemInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b1000;
    }
    virtual int getMOD()
    {
        return 0b0010;
    }

private:
    static StoreMemInstructionExecutor *instance;
};

//-------------------------------------------StoreWithUpdateInstructionExecutor----------------------------------------------//
class StoreWithUpdateInstructionExecutor : public InstructionExecutor
{
public:
    StoreWithUpdateInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b1000;
    }
    virtual int getMOD()
    {
        return 0b0001;
    }

private:
    static StoreWithUpdateInstructionExecutor *instance;
};

//-------------------------------------------LoadGprFromCsrInstructionExecutor----------------------------------------------//
class LoadGprFromCsrInstructionExecutor : public InstructionExecutor
{
public:
    LoadGprFromCsrInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b1001;
    }
    virtual int getMOD()
    {
        return 0b0000;
    }

private:
    static LoadGprFromCsrInstructionExecutor *instance;
};

//-------------------------------------------LoadGprFromGprInstructionExecutor----------------------------------------------//
class LoadGprFromGprInstructionExecutor : public InstructionExecutor
{
public:
    LoadGprFromGprInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b1001;
    }
    virtual int getMOD()
    {
        return 0b0001;
    }

private:
    static LoadGprFromGprInstructionExecutor *instance;
};

//-------------------------------------------LoadGprFromMemInstructionExecutor----------------------------------------------//
class LoadGprFromMemInstructionExecutor : public InstructionExecutor
{
public:
    LoadGprFromMemInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b1001;
    }
    virtual int getMOD()
    {
        return 0b0010;
    }

private:
    static LoadGprFromMemInstructionExecutor *instance;
};

//-------------------------------------------LoadPopInstructionExecutor----------------------------------------------//
class LoadPopInstructionExecutor : public InstructionExecutor
{
public:
    LoadPopInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b1001;
    }
    virtual int getMOD()
    {
        return 0b0011;
    }

private:
    static LoadPopInstructionExecutor *instance;
};

//-------------------------------------------LoadCsrFromGprInstructionExecutor----------------------------------------------//
class LoadCsrFromGprInstructionExecutor : public InstructionExecutor
{
public:
    LoadCsrFromGprInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b1001;
    }
    virtual int getMOD()
    {
        return 0b0100;
    }

private:
    static LoadCsrFromGprInstructionExecutor *instance;
};

//-------------------------------------------LoadCsrOrDispInstructionExecutor----------------------------------------------//
class LoadCsrOrDispInstructionExecutor : public InstructionExecutor
{
public:
    LoadCsrOrDispInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b1001;
    }
    virtual int getMOD()
    {
        return 0b0101;
    }

private:
    static LoadCsrOrDispInstructionExecutor *instance;
};

//-------------------------------------------LoadCsrFromMemInstructionExecutor----------------------------------------------//
class LoadCsrFromMemInstructionExecutor : public InstructionExecutor
{
public:
    LoadCsrFromMemInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b1001;
    }
    virtual int getMOD()
    {
        return 0b0110;
    }

private:
    static LoadCsrFromMemInstructionExecutor *instance;
};

//-------------------------------------------LoadPopCsrInstructionExecutor----------------------------------------------//
class LoadPopCsrInstructionExecutor : public InstructionExecutor
{
public:
    LoadPopCsrInstructionExecutor() : InstructionExecutor()
    {
        addExecutorToFactory();
    }
    void execute(Emulator &e, int regA, int regB, int regC, int disp);

protected:
    virtual int getOC()
    {
        return 0b1001;
    }
    virtual int getMOD()
    {
        return 0b0111;
    }

private:
    static LoadPopCsrInstructionExecutor *instance;
};

#endif /* INSTRUCTION_H */
