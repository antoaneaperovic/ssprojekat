#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>
#include <exception>


using namespace std;

class Exception : public exception
{
private:
    string message;
    const int errorCode;
    const int line;
    const string code;
public:
    explicit Exception(const std::string &_message, const int _errorCode = 0, const int _line = -1, const string& _code = "") : message(_message), errorCode(_errorCode), line(_line), code(_code) {}
    virtual ~Exception() {}

    const char *what() const noexcept override
    {
        return message.c_str();
    }

    virtual const string getErrorMessage()
    {
        string fullMessage = "\n";
        fullMessage += "===============================================================\n";
        fullMessage += "*** ERROR OCCURED ***\n";
        if (line != -1)
        {
            fullMessage += "Line number: " + to_string(line) + "\n";
            fullMessage += "Line: " + code + "\n";
        }
        fullMessage += "Error message: " + message + "\n";
        fullMessage += "Error code: " + to_string(errorCode) + "\n";
        fullMessage += "===============================================================\n";
        return fullMessage;
    }

    virtual const int getErrorCode()
    {
        return errorCode;
    }
};

#endif /* EXCEPTION_H */
