#ifdef _WIN32
#pragma once
#include <string>
#include <atlbase.h>
template <typename T> class StringUtilBase
{
public:
    static T BSTR2StdString(const BSTR bs);
};
#endif
