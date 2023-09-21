#ifndef SHARED_H
#define SHARED_H

#include <iostream>
#include <string>
#include <regex>
#include "exception.hpp"

using namespace std;

string getRegisterNum(string reg);

string getInstructionMask(string instruction);

string getInstructionMaskMMMM(string instruction, bool second);

string revertHex(string hexNum);

string decToHex(int decNum, bool addZeros);

string decToHex1(int decNum);

string decToHexNegativeNum(int decNum);

int hexToDec(string hexNum);
uint32_t hexToDecEmulator(string hexNum);

string clearSpaces(string line);

string getOperandAddressingType(string elemName);

string addSpace(int num);


#endif