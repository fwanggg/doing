#ifdef _WIN32
#include "StringUtil.h"
std::wstring StringUtilBase::BSTR2StdString(const BSTR bs)
{
    std::wstring ws(bs, SysStringLen(bs));
    return ws;
}
std::string StringUtilBase::ConvertUnicodeToUTF8(std::wstring unicode_str)
{
    wchar_t* unicode_c_str = nullptr;
    char* utf8_c_str = nullptr;
    ConvertUnicodeToUTF8(unicode_str.c_str(), &utf8_c_str);
    std::string ret = utf8_c_str; // deep copy
    if(utf8_c_str) free(utf8_c_str);
    return ret;
}
// CALLER release the memory of pszBuffer upon PM_RESULT_SUCCESS
// If any error happens inside this function, this function is responsible for realease any resource that has been allocated
bool StringUtilBase::ConvertUnicodeToUTF8(
    LPCWSTR unicode_c_str,
    LPSTR*  utf_buffer_c
)
{
    bool r = false;
    int iRet;
    LPSTR szUtf8CmdLine = NULL;
    if (utf_buffer_c == 0 || unicode_c_str == 0)
    {
        return r;
    }
    /*
    Determine the size (in chars) of the buffer needed
    for the UTF-8 version of the command line.
    */
    iRet = WideCharToMultiByte(
        CP_UTF8,
        0,
        unicode_c_str,
        -1,
        NULL,
        0,
        NULL,
        NULL);
    if (iRet)
    {
        szUtf8CmdLine = (LPSTR)malloc((iRet + 1) * sizeof(CHAR));
        if (szUtf8CmdLine)
        {
            /*
            Convert the string now that the buffer has been
            successfully allocated.
            */
            iRet = WideCharToMultiByte(
                CP_UTF8,
                0,
                unicode_c_str,
                -1,
                szUtf8CmdLine,
                iRet + 1,
                NULL,
                NULL);
            if (iRet)
            {
                r = true;
                _ASSERT(utf_buffer_c);
                *utf_buffer_c = szUtf8CmdLine;
            }
            else
            {
                r = false;
                free(szUtf8CmdLine);
            }
        }
        else
        {
            r = false;
        }
    }
    else
    {
        r = false;
    }


    return r;
}
#endif
//end of the file