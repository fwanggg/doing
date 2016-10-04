#include "Win32Util.h"
#include <Windows.h>
#include <string>
#include "TlHelp32.h"
#include <vector>
#ifdef _WIN32

std::wstring Win32Util<std::wstring>::GetWindowTitleByHandle(HWND hwnd)
{
    wchar_t win_text[1024] = { 0 };
    GetWindowText(hwnd, win_text, 1024);
    return win_text;
}
std::wstring Win32Util<std::wstring>::GetMachineName()
{
    DWORD buf_size = MAX_COMPUTERNAME_LENGTH + 1;
    wchar_t buf[MAX_COMPUTERNAME_LENGTH + 1] = { 0 };
    GetComputerName(buf, &buf_size);
    return buf;

}
std::wstring Win32Util<std::wstring>::GetProcName(unsigned int pid)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    std::wstring proc_name;
    if (hSnapshot) {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe32)) {
            do {
                if (pe32.th32ProcessID == pid)
                {
                    proc_name = pe32.szExeFile;
                    break;
                }
            } while (Process32Next(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
    return proc_name;
}
#else
#endif