#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <memory>
#include <iomanip>

#include "section1.hpp"
#include "symbol1.hpp"
#include "shared.hpp"
#include "exception.hpp"

using namespace std;

const string LITERAL_HEX = "(0x[0-9A-Fa-f]+)";
const string LITERAL__DEC_UNSIGNED = "([0-9]+)";
const string LITERAL = "(((-|\\+)?[0-9]+)|" + LITERAL_HEX + ")";
const string LITERAL_UNSIGNED = "(" + LITERAL__DEC_UNSIGNED + "|" + LITERAL_HEX + ")";
const string SYMBOL = "([a-zA-Z][a-zA-Z0-9_]*)";
const string REGISTER = "(%(r(1[0-5]|[0-9])|sp|pc))";
const string CXR_REGISTER = "(%(csr[0-2]|status|handler|cause))";
const string OPERAND = "((\\$" + LITERAL + "|\\$?" + SYMBOL + "|" + LITERAL_UNSIGNED + ")|" + REGISTER + "|\\[" + REGISTER + "(((\\+|-)" + LITERAL_UNSIGNED + ")|(\\+" + LITERAL_HEX + "))?\\])";
const regex LABEL = regex("^" + SYMBOL + ":(.*)?$");


const regex INSTRUCTION_HALT_INT_IRET_RET = regex("^(ret|int|iret|halt)$");
const regex INSTRUCTION_PUSH_POP = regex("^(push|pop) (" + REGISTER + ")$");
const regex INSTRUCTION_STORE = regex("^(st) " + REGISTER + "," + OPERAND + "$");
const regex INSTRUCTION_LOAD = regex("^(ld) ((\\$(((-|\\+)?[0-9]+)|(0x[0-9A-Fa-f]+))|\\$?([a-zA-Z][a-zA-Z0-9_]*)|(([0-9]+)|(0x[0-9A-Fa-f]+)))|(%(r(1[0-5]|[0-9])|sp|pc))|\\[(%(r(1[0-5]|[0-9])|sp|pc))(((\\+|-)([0-9]+))|(\\+(0x[0-9A-Fa-f]+)))?\\]),(%(r(1[0-5]|[0-9])|sp|pc))$");
const regex INSTRUCTION_CALL = regex("^(call) (" + LITERAL + "|" + SYMBOL + ")$");      
const regex INSTRUCTION_XCHG = regex("^(xchg) " + REGISTER + "," + REGISTER + "$");
const regex INSTRUCTION_JMP = regex("^(jmp) (" + LITERAL + "|" + SYMBOL + ")$");  
const regex INSTRUCTION_SHL_SHR= regex("^(shl|shr) " + REGISTER + "," + REGISTER + "$");
const regex INSTRUCTION_BEQ_BNE_BGT = regex("^(beq|bne|bgt) " + REGISTER + "," + REGISTER + ",(" + LITERAL + "|" + SYMBOL + ")$");
const regex INSTRUCTION_ADD_SUB_MUL_DIV = regex("^(add|sub|mul|div) " + REGISTER + "," + REGISTER + "$");
const regex INSTRUCTION_NOT = regex("^(not) " + REGISTER + "$");
const regex INSTRUCTION_AND_OR_XOR = regex("^(and|or|xor) " + REGISTER + "," + REGISTER + "$");
const regex INSTRUCTION_CSRRD = regex("^(csrrd) " + CXR_REGISTER + "," + REGISTER + "$");
const regex INSTRUCTION_CSRWR = regex("^(csrwr) " + REGISTER + "," + CXR_REGISTER + "$");

const regex DIRECTIVE_WORD = regex("^^\\.word ((" + SYMBOL + "|" + LITERAL + ")(,(" + SYMBOL + "|" + LITERAL + "))*)$");
const regex DIRECTIVE_SKIP = regex("^\\.skip " + LITERAL_UNSIGNED + "$");
const regex DIRECTIVE_GLOBAL = regex("^\\.global (" + SYMBOL + "(," + SYMBOL + ")*)$");
const regex DIRECTIVE_EXTERN = regex("^\\.extern (" + SYMBOL + "(," + SYMBOL + ")*)$");
const regex DIRECTIVE_SECTION = regex("^\\.section " + SYMBOL + "$");
const regex DIRECTIVE_END = regex("^\\.end$");

class Assembler
{
public:

    explicit Assembler(string fileInputName, string fileOutputName) {
        this->fileInputName = fileInputName;
        this->fileOutputName = fileOutputName;
    }
    virtual ~Assembler() = default;

    virtual void execute();

private:
    virtual void generateFirstPass();
    virtual void generateSecondPass();
    virtual void addNewSymbolToCurrentSection(int symbID, int value, string symbName, string symbType, int sectID, string sectName, bool isDef);

    virtual void openFileAndReadData();

    virtual void printALLSymbols(ostream& os);
    virtual void printALLSections(ostream& os);

    virtual void openFileAndWriteData();

    virtual void printALLSymbolsBin(ofstream& fileOutputBin);
    virtual void printALLSectionsBin(ofstream& fileOutputBin);

    virtual void openFileAndWriteDataBin();

    virtual void changeSymbolsOffset();
    virtual void changeLiteralsOffset();
    virtual void checkSymbolsDefinition();
    virtual void putLiteralsToTheEnd();
    virtual void putSymbolsToTheEnd();
    virtual void addLiteralHex(shared_ptr<Section> tempSection, string hexNum);
    virtual void createInstructionCode(shared_ptr<Section> tempSection, string instruction, string op1, string op2, string c, int d, bool isSecond);    

    vector<string> lines;
    string fileInputName;
    string fileOutputName;
    unordered_map<string, shared_ptr<Symbol>> symbols;
    unordered_map<string, shared_ptr<Section>> sections;
};
#endif 

