#ifdef _WIN32
#include "StringUtil.h"
std::wstring StringUtilBase<std::wstring>::BSTR2StdString(const BSTR bs)
{
    std::wstring ws(bs, SysStringLen(bs));
    return ws;
}
#endif
//end of the file