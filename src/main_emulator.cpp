#include <iostream>
#include <exception>
#include "exception.hpp"
#include "emulator.hpp"

using namespace std;

int main(int argc, const char *argv[])
{
    try
    {   
        string fileInput = argv[1];
        if(fileInput == "" ) {
            throw Exception("Niste uneli nazive fajlova.");
        }

        if(argc != 2) {
         //   throw Exception("Uneti su pogresni parametri za pokretanje emulatora.");
        }

        Emulator emulator(fileInput);

        emulator.emulate();

        
        cout << "Uspesno radi sve za emulator!" << endl;
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
