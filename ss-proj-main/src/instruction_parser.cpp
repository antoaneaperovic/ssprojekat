#include "instruction_parser.hpp"

InstructionParser::InstructionParser(vector<uint8_t> bytes)
{
    this->bytes = bytes;

    oc = (bytes[0] >> 4) & 0xf;
    mod = bytes[0] & 0xf;

    regA = (bytes[1] >> 4) & 0xf;
    regB = bytes[1] & 0xf;
    regC = (bytes[2] >> 4) & 0xf;

    int dispSign = (bytes[2] >> 3) & 0x1;

    disp = ((int) bytes[2] & 0xf) << 8 | ((int) bytes[3]);

    if(dispSign == 1) {
        disp |= 0xfffff000;
    }
}
