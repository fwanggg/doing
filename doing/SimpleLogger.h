#pragma once
#include <string>
enum LOGGER_LEVEL
{
    INFO_LVL,
    WARNING_LVL,
    ERROR_LVL
};
class SimpleLogger
{   
public:

    SimpleLogger(std::wstring file_name);
    static SimpleLogger GetInstance();
    void WriteLogMessage(LOGGER_LEVEL level, std::wstring msg);
private:
    std::wstring _file_name;
};

static const wchar_t * EnumStrings[] = { L"INFO", L"WARNING", L"ERROR" };
