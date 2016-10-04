#pragma once
#ifdef _WIN32
#include <Windows.h>
template<typename T> class Win32Util
{
public:
    static T GetProcName(unsigned int pid);
    static T GetMachineName();
    static T GetWindowTitleByHandle(HWND hwnd);
};
#endif
