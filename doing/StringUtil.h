#ifdef _WIN32
#pragma once
#include <string>
#include <atlbase.h>
class StringUtilBase
{
public:
    static std::wstring BSTR2StdString(const BSTR bs);
    static std::string ConvertUnicodeToUTF8(std::wstring unicode_str);
private:
    static bool ConvertUnicodeToUTF8(
        LPCWSTR szWideCmdLine,
        LPSTR*  pszBuffer
    );
};
#endif
