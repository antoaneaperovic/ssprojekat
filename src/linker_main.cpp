#include <iostream>
#include <vector>
#include <exception>
#include <string>
#include "exception.hpp"
#include "linker.hpp"
#include "place_item.hpp"


using namespace std;

int main(int argc, const char *argv[])
{
    try {

        bool isHex = false;
        unordered_map<string, shared_ptr<PlaceItem>> placeItemsList;
        string fileOutputName;
        vector<string> fileInputNames;
        int fileInputSize = 0;

        for(int i = 1; i < argc; i++) {
            string text = argv[i];
            if(text.find("hex") != string::npos) {
                isHex = true;
            }
            else if(text.find("place") != string::npos) {
                shared_ptr<PlaceItem> placeItem = make_shared<PlaceItem>(argv[i]);
                placeItemsList[placeItem->getSection()] = placeItem;
            }
            else if(text == "-o") {
                i++;
                fileOutputName = argv[i];
                i++;
                while(i < argc) {
                    string fileIN = argv[i];
                    if(fileIN.back() == 'o') {
                        fileInputNames.push_back(argv[i]);
                        fileInputSize++;
                    }
                    i++;
                }

            }
        }

        // dodaj proveru za parametre

        Linker linker(placeItemsList, fileInputNames, fileOutputName, isHex, fileInputSize);
        linker.link();

        cout << "Uspesno zavrsen linker!" << endl;
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
