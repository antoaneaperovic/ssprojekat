#include <iostream>
#include <string>
#include <regex>
#include "shared.hpp"
#include "exception.hpp"

using namespace std;

const regex REGEX_COMMENT("([^#]*)#.*");
const regex REGEX_SPACES(" {2,}");
const regex REGEX_TABS("(\\n|\\r|\\t|\\f|\\v)");
const regex REGEX_SPECIAL_CHARACTERS_SPACES(" *(,|:|\\+|-) *");
const regex REGEX_SPECIAL_CHARACTERS_RIGHT_SPACES("(\\[|//$|\\%) *");
const regex REGEX_SPECIAL_CHARACTERS_LEFT_SPACES(" *(\\])");
const regex REGEX_TRIM("^( *)(.*[^ ])( *)$");

string getRegisterNum(string reg) {
    cout << "REGISTAR: " << reg << endl;
    if(reg == "") return "0"; 
    if(reg.at(0) == '%') {
        reg = reg.substr(1);
    }
    if(reg.at(0) == 'r') {
        return decToHex1(stoi(reg.substr(1)));
    }
    else {
        if(reg == "status") {
            return "0";
        }
        if(reg == "handler") {
            return "1";
        }
        if(reg == "cause") {
            return "2";
        }
        if(reg == "pc") {
            return "F";
        }
        if(reg == "sp") {
            return "E";
        }
    }
    return "";
}

string getInstructionMask(string instruction) {
    if(instruction == "halt") return "0";
    if(instruction == "jmp" || instruction == "bgt" ||instruction == "bne" || instruction == "beq") return "3";
    if(instruction == "int") return "1";
    if(instruction == "iret" || instruction == "ret" || instruction == "pop" || instruction == "ld" || instruction == "csrrd" || instruction == "csrwr") return "9";
    if(instruction == "call") return "2";
    if(instruction == "push" || instruction == "st") return "8";
    if(instruction == "xchg") return "4";
    if(instruction == "add" || instruction == "sub" ||instruction == "mul" || instruction == "div") return "5";
    if(instruction == "not" || instruction == "and" ||instruction == "or" || instruction == "xor") return "6";
    if( instruction == "shl" || instruction == "shr") return "7";
    return "";
}

string getInstructionMaskMMMM(string instruction, bool second) {
    if(second) {
        if(instruction == "ld" || instruction == "st") return "2";
        if(instruction == "iret") return "3";
        if(instruction == "jmp") return "8";
        if(instruction == "beq") return "9";
        if(instruction == "bne") return "A";
        if(instruction == "bgt") return "B";
        if(instruction == "call") return "1";
    }
    else {
        if(instruction == "halt" || instruction == "int" || instruction == "add" || instruction == "not" 
            || instruction == "csrrd" || instruction == "call" ||instruction == "jmp" || instruction == "shl" || instruction == "st" || instruction == "xchg") 
            return "0";
        if(instruction == "beq" || instruction == "push" || instruction == "sub" || instruction == "and" || instruction == "ld" || instruction == "shr") return "1";
        if(instruction == "bne" || instruction == "mul" || instruction == "or") return "2";
        if(instruction == "ret" || instruction == "pop" || instruction == "div" || instruction == "xor" || instruction == "bgt") return "3";
        if(instruction == "csrwr") return "4";
        if(instruction == "iret") return "6"; // ili 5 ili 6 proveri
    
    }
    return "";
}

string revertHex(string normalhex){
    string revertedhex = normalhex.substr(6, 2) + normalhex.substr(4, 2) + normalhex.substr(2, 2) + normalhex.substr(0, 2);
    return revertedhex;
}

string decToHex1(int decNum) {
    switch (decNum)
    {
    case 0:
        return "0";
        break;

    case 1:
        return "1";
        break;
    case 2:
        return "2";
        break;
    case 3:
        return "3";
        break;
    case 4:
        return "4";
        break;    
    case 5:
        return "5";
        break;
    case 6:
        return "6";
        break;
    case 7:
        return "7";
        break;
    case 8:
        return "8";
        break;
    case 9:
        return "9";
        break;
    case 10:
        return "A";
        break;
    case 11:
        return "B";
        break;
    case 12:
        return "C";
        break;
    case 13:
        return "D";
        break;
    case 14:
        return "E";
        break;
    case 15:
        return "F";
        break;    
    default:
        return "";
        break;
    }
}

string decToHex(int decNum, bool addZeros = false) {
    if(decNum == 0) return "0";
    string hexNum = "";
    int num;
    if(decNum < 0) return decToHexNegativeNum(decNum);
    while(decNum > 0) {
        num = decNum % 16;
        decNum = decNum / 16;
         switch (num) {
            case 10:
                hexNum += "A";
                break;
            case 11:
                hexNum += "B";
                break;
            case 12:
                hexNum += "C";
                break;
            case 13:
                hexNum += "D";
                break;
            case 14:
                hexNum += "E";
                break;
            case 15:
                hexNum += "F";
                break;    
            default:
                hexNum += to_string(num);
                break;
        }
    }
    if(addZeros) {
        while(hexNum.length() < 8) {
            hexNum+="0";
        }
    }
    reverse(hexNum.begin(), hexNum.end());
    return hexNum;
}

string decToHexNegativeNum(int num) {
    int decNum = num*(-1);
    string hexNum = "";
    uint32_t numUINT = static_cast<uint32_t>(num);
    while(numUINT) {
        hexNum += decToHex1(numUINT%16);
        numUINT/=16;
    }
    reverse(hexNum.begin(), hexNum.end());
    return hexNum;
}

string clearSpaces(string line) {

    string newLine = regex_replace(line, REGEX_COMMENT, "$1");
    newLine = regex_replace(newLine, REGEX_TABS, " ");
    newLine = regex_replace(newLine, REGEX_SPACES, " ");
    newLine = regex_replace(newLine, REGEX_SPECIAL_CHARACTERS_SPACES, "$1");
    newLine = regex_replace(newLine, REGEX_SPECIAL_CHARACTERS_LEFT_SPACES, "$1");
    newLine = regex_replace(newLine, REGEX_SPECIAL_CHARACTERS_RIGHT_SPACES, "$1");
    newLine = regex_replace(newLine, REGEX_TRIM, "$2");

    return newLine != " " ? newLine : "";
}

string getOperandAddressingType(string elemName)
{
    if(elemName.at(0) == '$')  return isalpha(elemName.at(1)) != 0 ? "Symbol" : "Literal";
    if(elemName.at(0) == '%')  return "Register";
    if(elemName.at(0) == '[') {
        int index = elemName.find('+') != string::npos ? elemName.find('+') : elemName.find('-');
        if(index == string::npos)   return "Indirect_Register";
        else {
            string pom = elemName.substr(index, elemName.length() - 1);
            int pom_value = hexToDec(pom);
            return pom_value == 0 ? "Indirect_Register" : "Indirect_Register_Pom";
        }
    }
    else {
        cout << "ISPIS ZA MEM_SYM            " << elemName << endl;
        return isalpha(elemName.at(0)) != 0 ? "Mem_Symbol" : "Mem_Literal";
    }
}

string addSpace(int num) {
        if(num < 10) return "       ";
        if(num < 100) return "      ";
        return "    ";
}

int hexToDec(string hexNum) {
    if(hexNum.at(0) == '$') hexNum = hexNum.substr(1);
    if(hexNum.find('x') == -1) return stoi(hexNum);
    size_t sz = 0;
    cout << "BROJ:  " << hexNum;
    if(hexNum.at(0) == '$') hexNum = hexNum.substr(1);
    if(hexNum.empty()) return 0;
    cout << "EVO:  " << hexNum;
    int num = stoll(hexNum, &sz, 16);
    return num;
}

uint32_t hexToDecEmulator(string hexNum) {
    uint32_t num = stoll(hexNum, nullptr, 16);
    return num;
}