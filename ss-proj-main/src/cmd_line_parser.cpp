#include "cmd_line_parser.hpp"
#include "exception.hpp"

const string CmdLineParser::INPUT_FILE_NAME = "input_file";
const string CmdLineParser::OUTPUT_FILE_NAME = "output_file";
const string CmdLineParser::OUTPUT_HEX_FILE_NAME = "output_HEX_file";
const string CmdLineParser::NUMBER_OF_INPUT_FILES = "number_of_input_files";
const string CmdLineParser::PROG_NAME = "prog_name";
const string CmdLineParser::HEX = "hex";
const string CmdLineParser::RELOCATABLE = "relocatable";
const string CmdLineParser::NUMBER_OF_PLACES = "number_of_places";
const string CmdLineParser::PLACE = "place";

void CmdLineParser::parse()
{
    cout << argc << endl;
    for (int i = 0; i < argc; i++)
    {
        cout << i << ": " << argv[i] << endl;
    }
    cout << endl;

    args[PROG_NAME] = argv[0];

    int index = 1;

    args[NUMBER_OF_PLACES] = "0";
    while (index < argc)
    {
        if (argv[index] == "-o")
        {
            index++;
            args[OUTPUT_FILE_NAME] = argv[index];
            int filePosition = 0;
            while (++index < argc)
            {
                args[INPUT_FILE_NAME + "_" + to_string(filePosition)] = argv[index];
                filePosition++;
            }
            args[NUMBER_OF_INPUT_FILES] = to_string(filePosition);
            index--;
            return;
        }
        else if (argv[index] == "-hex")
        {
            args[HEX] = "1";
        }
        else if (argv[index] == "-relocatable")
        {
            args[RELOCATABLE] = "1";
        }
        else if (argv[index].find("-place=") == 0)
        {
            args[PLACE + "_" + args[NUMBER_OF_PLACES]] = argv[index];
            args[NUMBER_OF_PLACES] = to_string(stoi(args[NUMBER_OF_PLACES]) + 1);
        }
        index++;
    }

    args[OUTPUT_HEX_FILE_NAME] = argv[1];
}
