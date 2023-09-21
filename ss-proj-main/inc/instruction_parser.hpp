#ifndef INSTRUCTION_PARSER_H
#define INSTRUCTION_PARSER_H

#include <iostream>
#include <vector>

using namespace std;

class InstructionParser
{
public:
    InstructionParser(vector<uint8_t> bytes);

    int getOc() const
    {
        return oc;
    }
    int getMod() const
    {
        return mod;
    }
    int getRegA() const
    {
        return regA;
    }
    int getRegB() const
    {
        return regB;
    }
    int getRegC() const
    {
        return regC;
    }
    int getDisp() const
    {
        return disp;
    }

    string getData()
    {
        return "oc: " + to_string(oc) + "; mod: " + to_string(mod) + "; regA: " + to_string(regA) + "; regB: " + to_string(regB) + "; regC: " + to_string(regC) + "; disp: " + to_string(disp);
    }

private:
    vector<uint8_t> bytes;
    int oc = 0;
    int mod = 0;
    int regA = 0;
    int regB = 0;
    int regC = 0;
    int disp = 0;
};

#endif /* INSTRUCTION_PARSER_H */
