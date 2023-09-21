#include "instruction_executor_factory.hpp"

InstructionExecutorFactory *InstructionExecutorFactory::instance = nullptr;

InstructionExecutorFactory *InstructionExecutorFactory::getInstance()
{
    if (instance == nullptr)
    {
        instance = new InstructionExecutorFactory();
    }
    return instance;
}

void InstructionExecutorFactory::addInstructionExecutor(InstructionExecutor* instructionExecutor, int oc, int mod)
{
    if (executors.count(oc) == 1 && executors[oc].count(mod) == 1)
    {
         throw runtime_error("Instruction with same oc(" + to_string(oc) + ") and mod (" + to_string(mod) + ") exists!");
    }

    executors[oc][mod] = instructionExecutor;
}

InstructionExecutor* InstructionExecutorFactory::getInstructionExecutor(int oc, int mod)
{
    if (executors.count(oc) == 1 && executors[oc].count(mod) == 1)
    {
        return executors[oc][mod];
    }

    return nullptr;
}
