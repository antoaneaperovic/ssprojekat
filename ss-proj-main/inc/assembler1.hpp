#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <memory>
#include <iomanip>

#include "section.hpp"
#include "symbol.hpp"
#include "utils.hpp"
#include "exception.hpp"

using namespace std;

class Assembler
{

private:
    virtual void initFirstPass();
    virtual void initSecondPass();
    virtual void firstPass();
    virtual void secondPass();
    virtual void checkIfTablesAreCreatedSuccessfully();
    virtual void addNewSymbolToCurrentSection();

    virtual OperandType getOperandType(string operand);
    virtual ValueType getValueType(string data);

    virtual vector<unsigned int> generateBytes(unsigned int op, unsigned int mod, unsigned int ra, unsigned int rb, unsigned int rc, int disp);
    virtual vector<unsigned int> generateBytes(int number);

    vector<string> instructions;
    unordered_map<string, shared_ptr<Symbol>> symbolTable;
    unordered_map<string, shared_ptr<Section>> sectionTable;

    static unordered_map<string, unsigned int> REGISTER_MAP;
    static unordered_map<string, unsigned int> INSTRUCTION_OP_MAP;
    static unordered_map<string, vector<unsigned int>> INSTRUCTION_MMMM_MAP;
public:
    enum OperandType
    {
        LITERAL_VAL,
        SYMBOL_VAL,
        LITERAL_MEM_DIR,
        SYMBOL_MEM_DIR,
        REG_DIR,
        REG_INDIR,
        REG_INDIR_DISPL,
    };

    enum ValueType
    {
        LITERAL,
        SYMBOL,
    };

    explicit Assembler(const vector<string> &_instructions) : instructions(_instructions) {}
    virtual ~Assembler() = default;

    virtual void execute();
    virtual vector<shared_ptr<Symbol>> getSymbols() const;
    virtual vector<shared_ptr<Section>> getSections() const;
};

const string LITERAL_HEX_DEFINITION = "(0x[0-9A-Fa-f]+)";
const string LITERAL_UNSIGNED_DEC_DEFINITION = "([0-9]+)";
const string LITERAL_DEFINITION = "(((-|\\+)?[0-9]+)|" + LITERAL_HEX_DEFINITION + ")";
const string UNSIGNED_LITERAL_DEFINITION = "(" + LITERAL_UNSIGNED_DEC_DEFINITION + "|" + LITERAL_HEX_DEFINITION + ")";
const string SYMBOL_DEFINITION = "([a-zA-Z][a-zA-Z0-9_]*)";
const string GPR_DEFINITION = "(%(r(1[0-5]|[0-9])|sp|pc))";
const string CXR_DEFINITION = "(%(csr[0-2]|status|handler|cause))";
// const string OPERAND_DEFINITION = "((\\$?" + LITERAL_DEFINITION + "|\\$?" + SYMBOL_DEFINITION + ")|" + GPR_DEFINITION + "|\\[" + GPR_DEFINITION + "((\\+|-)" + UNSIGNED_LITERAL_DEFINITION + ")?\\])";

const string OPERAND_DEFINITION = "((\\$" + LITERAL_DEFINITION + "|\\$?" + SYMBOL_DEFINITION + "|" + UNSIGNED_LITERAL_DEFINITION + ")|" + GPR_DEFINITION + "|\\[" + GPR_DEFINITION + "(((\\+|-)" + LITERAL_UNSIGNED_DEC_DEFINITION + ")|(\\+" + LITERAL_HEX_DEFINITION + "))?\\])";
// ((\$(((-|\+)?[0-9]+)|(0x[0-9A-Fa-f]+))|\$?([a-zA-Z][a-zA-Z0-9_]*)|(([0-9]+)|(0x[0-9A-Fa-f]+)))|(%(r(1[0-5]|[0-9])|sp|pc))|\[(%(r(1[0-5]|[0-9])|sp|pc))(((\+|-)([0-9]+))|(\+(0x[0-9A-Fa-f]+)))?\])

const regex REGEX_LABEL = regex("^" + SYMBOL_DEFINITION + ":(.*)?$");
const regex REGEX_GLOBAL_DIRECTIVE = regex("^\\.global (" + SYMBOL_DEFINITION + "(," + SYMBOL_DEFINITION + ")*)$");
const regex REGEX_EXTERN_DIRECTIVE = regex("^\\.extern (" + SYMBOL_DEFINITION + "(," + SYMBOL_DEFINITION + ")*)$");
const regex REGEX_SECTION_DIRECTIVE = regex("^\\.section " + SYMBOL_DEFINITION + "$");
const regex REGEX_END_DIRECTIVE = regex("^\\.end$");
const regex REGEX_WORD_DIRECTIVE = regex("^^\\.word ((" + SYMBOL_DEFINITION + "|" + LITERAL_DEFINITION + ")(,(" + SYMBOL_DEFINITION + "|" + LITERAL_DEFINITION + "))*)$");
const regex REGEX_SKIP_DIRECTIVE = regex("^\\.skip " + UNSIGNED_LITERAL_DEFINITION + "$");
const regex REGEX_ASCII_DIRECTIVE = regex("^\\.ascii \\\"(.*)\\\"$");

const regex REGEX_NO_OP_INSTRUCTIONS = regex("^(halt|int|iret|ret)$");

const regex REGEX_STACK_INSTRUCTIONS = regex("^(push|pop) (" + GPR_DEFINITION + ")$");

const regex REGEX_UNCONDITIONAL_JUMP_INSTRUCTIONS = regex("^(call|jmp) (" + LITERAL_DEFINITION + "|" + SYMBOL_DEFINITION + ")$");                                                // 1,2
const regex REGEX_CONDITIONAL_JUMP_INSTRUCTIONS = regex("^(beq|bne|bgt) " + GPR_DEFINITION + "," + GPR_DEFINITION + ",(" + LITERAL_DEFINITION + "|" + SYMBOL_DEFINITION + ")$"); // 1,2,5,8

const regex REGEX_ARITHMETIC_INSTRUCTIONS = regex("^(add|sub|mul|div) " + GPR_DEFINITION + "," + GPR_DEFINITION + "$"); // 1,2,5

const regex REGEX_NOT_INSTRUCTION = regex("^(not) " + GPR_DEFINITION + "$");
const regex REGEX_BITWISE_INSTRUCTIONS = regex("^(and|or|xor|shl|shr) " + GPR_DEFINITION + "," + GPR_DEFINITION + "$"); // 1,2,5

const regex REGEX_XCHG_INSTRUCTION = regex("^(xchg) " + GPR_DEFINITION + "," + GPR_DEFINITION + "$"); // 1,2,5

const regex REGEX_CSRRD_INSTRUCTION = regex("^(csrrd) " + CXR_DEFINITION + "," + GPR_DEFINITION + "$"); // 1,2,5
const regex REGEX_CSRWR_INSTRUCTION = regex("^(csrwr) " + GPR_DEFINITION + "," + CXR_DEFINITION + "$"); // 1,2,5

const regex REGEX_LOAD_INSTRUCTION = regex("^(ld) " + OPERAND_DEFINITION + "," + GPR_DEFINITION + "$");
//^(ld) ((\$(((-|\+)?[0-9]+)|(0x[0-9A-Fa-f]+))|\$?([a-zA-Z][a-zA-Z0-9_]*)|(([0-9]+)|(0x[0-9A-Fa-f]+)))|(%(r(1[0-5]|[0-9])|sp|pc))|\[(%(r(1[0-5]|[0-9])|sp|pc))(((\+|-)([0-9]+))|(\+(0x[0-9A-Fa-f]+)))?\]),(%(r(1[0-5]|[0-9])|sp|pc))$

// 1 - ld, 2 - OPERAND_DEFINITION, 24 - GPR_DEFINITION
// OPERAND_DEFINITION = %rX             %rX - group 2
// OPERAND_DEFINITION = symbol,         symbol - group 8
// OPERAND_DEFINITION = $symbol,        symbol - group 8
// OPERAND_DEFINITION = 1111,           1111 - group 3
// OPERAND_DEFINITION = $1111,          1111 - group 4
// OPERAND_DEFINITION = [%r1] -         %r1 - group 15
// OPERAND_DEFINITION = [%r1-1111]-     %r1 - group 15,  -1111 - group 18 - koristi grup 18 za sve, jer sadrzi znak i za decimal i hex
const regex REGEX_STORE_INSTRUCTION = regex("^(st) " + GPR_DEFINITION + "," + OPERAND_DEFINITION + "$");
//^(st) (%(r(1[0-5]|[0-9])|sp|pc)),((\$?((-?[0-9]+)|(0x[0-9A-Fa-f]+))|\$?([a-zA-Z][a-zA-Z0-9_]*))|(%(r(1[0-5]|[0-9])|sp|pc))|\[(%(r(1[0-5]|[0-9])|sp|pc))((\+|-)(([0-9]+)|(0x[0-9A-Fa-f]+)))?\])$
// 1 - st, 2 - GPR_DEFINITION, 5 - OPERAND_DEFINITION
// OPERAND_DEFINITION = %rX             %rX - group 5
// OPERAND_DEFINITION = symbol,         symbol - group 11
// OPERAND_DEFINITION = $symbol,        symbol - group 11 - forbidden
// OPERAND_DEFINITION = 1111,           1111 - group 6
// OPERAND_DEFINITION = $1111,          1111 - group 7 - forbidden
// OPERAND_DEFINITION = [%r1] -         %r1 - group 18
// OPERAND_DEFINITION = [%r1-1111] -    %r1 - group 18, +1111 - group 21 - koristi grup 21 za sve, jer sadrzi znak i za decimal i hex
#endif /* ASSEMBLER_H */

/*
LITERAL_VAL - ako moze da stane na 12 bitova, tamo se upisuje sa sve znakom, ako ne, pravi se konstanta i koristi se pc relativno
SYMBOL_VAL - uvek je 4 bajta, koristi se pc relativno
LITERAL_MEM_DIR - uvek je 8 bajtova jer je adresa 4 bajta i koristi se pc relativno
SYMBOL_MEM_DIR - ako nije labela u tok sekciji, onda je instrukcija 8 bajtova - u oba slucaja se koristi pc relativno na kraj sekcije
*/
