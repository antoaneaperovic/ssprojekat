#include <iostream>
#include <vector>
#include <exception>
#include "exception.hpp"
#include "assembler.hpp"

using namespace std;

int main(int argc, const char *argv[])
{
    try
    {   
        string fileInput = argv[3];
        string fileOutput = argv[2];
        if(fileInput == "" || fileOutput == "") {
            throw Exception("Niste uneli nazive fajlova.");
        }

        Assembler assembler(fileInput, fileOutput);

        assembler.execute();

        
        cout << "Uspesno radi sve za asembler!" << endl;
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

    return 0;
}
