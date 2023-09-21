#ifndef INSTRUCTION_EXECUTOR_FACTORY_H
#define INSTRUCTION_EXECUTOR_FACTORY_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <unordered_map>

using namespace std;

class InstructionExecutor;

class InstructionExecutorFactory
{
public:
    static InstructionExecutorFactory *getInstance();
    void addInstructionExecutor(InstructionExecutor* instructionExecutor, int oc, int mod);

    InstructionExecutor* getInstructionExecutor(int oc, int mod);

private:
    static InstructionExecutorFactory *instance;

    unordered_map<int, unordered_map<int, InstructionExecutor*>> executors;
};

#endif /* INSTRUCTION_EXECUTOR_FACTORY_H */
