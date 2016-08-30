#pragma once

#include <string>
#include <cstdarg>
#include <ctime>
#include <memory>
#include <execinfo.h>
#include "LogElement.h"
#include "Logger.h"
#include "LogDefs.h"

const int ORIGINAL_STACK_FRAMES_GUESS = 20; //20 is arbitrary

template<typename ... Args>
std::string MergeFormat(const std::string& format, Args&... args)
{
    int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0', determine the specific size.
    std::unique_ptr<char[]> buf( new char[ size ] );
    snprintf( buf.get(), size, format.c_str(), args ... );
    std::string message( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
    return message;
}

/*Trace function will provide a mean to capture a requested log, its time and location with in the source files.
the function will bundle all the needed information and providfe it in an async manner to the logging thread
for future writing.*/
template<typename ... Args>
void Trace(const char* fileName, int line, const std::string& format, Args ... args)
{
    const time_t currentTime = time(0);
    std::string message = MergeFormat(format, args...);
    LogElement element(line, fileName, message, currentTime);
    Logger::Instance().Push(element);
}

template<typename ... Args>
void PrintStack(const char* fileName, int line, const std::string& format, Args&... args)
{
    //Trace the first message:
    Trace(fileName, line, format, args...);
    //Trace the entire stack frames:
    //Get the stack frames data
    void* stackFramesAddresses = malloc(sizeof(void*)*ORIGINAL_STACK_FRAMES_GUESS);
    int stackFramesSize = ORIGINAL_STACK_FRAMES_GUESS;
    int readFramesCount;
    while(stackFramesSize == (readFramesCount = backtrace(&stackFramesAddresses, stackFramesSize)))
    {
        free(stackFramesAddresses);
        stackFramesSize*=2;
        stackFramesAddresses = malloc(sizeof(void*)*stackFramesSize);
    }
    //Get the symbols
    char** stackFramesSymbols = backtrace_symbols(&stackFramesAddresses, readFramesCount);
    for(int index = 0; index < readFramesCount; index++)
    {
        Trace(fileName, line, std::string(stackFramesSymbols[index]));
    }
    free(stackFramesSymbols);
}
