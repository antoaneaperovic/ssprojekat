#ifndef CMD_LINE_PARSER_H
#define CMD_LINE_PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// dodaj i ostale argumenta za linker i emulator ako bude trebalo!!!
class CmdLineParser
{
private:
    unordered_map<string, string> args;
    int argc;
    vector<string> argv;

public:
    explicit CmdLineParser(const int _argc, const char **_argv) : argc(_argc), argv(vector<string>(_argv, _argv + _argc)) {
        parse();
    }
    virtual ~CmdLineParser() = default;

    void parse();
    virtual string get(const string& arg)
    {
        return args[arg];
    }

    static const string INPUT_FILE_NAME;
    static const string OUTPUT_FILE_NAME;
    static const string OUTPUT_HEX_FILE_NAME;
    static const string PROG_NAME;
    static const string NUMBER_OF_INPUT_FILES;
    static const string HEX;
    static const string RELOCATABLE;
    static const string NUMBER_OF_PLACES;
    static const string PLACE;

};

#endif /* CMD_LINE_PARSER_H */
