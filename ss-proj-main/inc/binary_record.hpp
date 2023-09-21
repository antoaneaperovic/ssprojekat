#ifndef BINARY_RECORD_H
#define BINARY_RECORD_H

#include <iostream>
#include <vector>

using namespace std;

class BinaryRecord
{
public:
    BinaryRecord(int _offset, vector<unsigned int> _bytes = {}) : offset(_offset), bytes(_bytes) {}
    void addByte(unsigned int byte)
    {
        bytes.push_back(byte);
    }
    unsigned int getOffset() const
    {
        return offset;
    }
    vector<unsigned int> getBytes() const
    {
        return bytes;
    }

private:
    unsigned int offset;
    vector<unsigned int> bytes;
};

#endif /* BINARY_RECORD_H */
