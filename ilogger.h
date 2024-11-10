#ifndef ILOGGER_H
#define ILOGGER_H

#include <string>

class ILogger
{ 
public:
    virtual ILogger& operator<< (const std::string& msg)
    {
        _logStream << msg;
        return *this;
    }

protected:
    ILogger(std::ostream& logStream) :
        _logStream(logStream)
    {}

private:
    std::ostream& _logStream;

};

#endif