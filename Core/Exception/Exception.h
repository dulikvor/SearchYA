#pragma once

#include <exception>
#include <string>
#include "Logger/Trace.h"
/**/
class Exception : public std::exception
{
public:
    /*Exception constructor will megerge the received exception message and print out the message and the entire
    stack frames to the log.*/
    template<typename ... Args>
    Exception(const char* fileName, int line, const std::string& format, Args ... args)
    {
        m_message = MergeFormat(format, args...);
        PrintStack(fileName, line, m_message);
    }

    Exception() = delete;
    virtual ~Exception(){}
    //Accessor
    std::string GetMessage() const {return m_message;}
private:
    std::string m_message;
};
