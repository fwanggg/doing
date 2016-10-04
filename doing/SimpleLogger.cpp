#include "SimpleLogger.h"
#include <mutex>
#include <fstream>
#include <ctime>
#include <iostream>
#include <time.h>
void SimpleLogger::WriteLogMessage(LOGGER_LEVEL level, std::wstring msg)
{
    static wchar_t buf[256] = { 0 };
    std::mutex m;
    m.lock();
    std::wofstream logger_file;
    logger_file.open(_file_name, std::wofstream::app | std::wofstream::out);
    std::time_t result = std::time(nullptr);
    _wasctime_s(buf, std::localtime(&result));
    logger_file << EnumStrings[level] << L", " << buf << L", " << msg << std::endl;
    m.unlock();
}
SimpleLogger SimpleLogger::GetInstance()
{
    static SimpleLogger logger(L"doing_log.log");
    return logger;
}
SimpleLogger::SimpleLogger(std::wstring file_name) : _file_name(file_name) 
{

}