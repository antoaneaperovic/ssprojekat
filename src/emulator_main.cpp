#include <iostream>
#include <vector>
#include <exception>
#include "exception.hpp"
#include "cmd_line_parser.hpp"
#include "file_reader.hpp"
#include "emulator.hpp"
#include "binary_record.hpp"

using namespace std;

int main(int argc, const char *argv[])
{
    try
    {
        CmdLineParser cmdLineParser(argc, argv);

        if (cmdLineParser.get(CmdLineParser::OUTPUT_HEX_FILE_NAME) == "")
        {
            throw Exception("Missing input file", 1);
        }

        FileReader fileReader(cmdLineParser.get(CmdLineParser::OUTPUT_HEX_FILE_NAME));

        vector<shared_ptr<BinaryRecord>> records = fileReader.readRecords();

        Emulator emulator(records);

        emulator.run();
        
        cout << endl;
        cout << "-----------------------------------------------------------------" << endl;
        cout << "Emulated processor executed halt instruction" << endl;
        cout << "Emulated processor state:";

        for (int i = 0; i < 16; i++)
        {
            if (i % 4 == 0)
            {
                cout << endl;
            }
            string space = i < 10 ? " " : "";
            cout << space << "r" << i << "=0x" << convert_int_to_hex_with_setw(emulator.getGPR(i), 8) << "\t";
        }
        cout << endl;
    }
    catch (Exception &e)
    {
        cout << e.getErrorMessage() << endl;
        return e.getErrorCode();
    }
    catch (exception &e)
    {
        cout << e.what() << endl;
        return -1;
    }
}
