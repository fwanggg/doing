#include "ProcInfoUtil.h"
#include <Windows.h>
#include <string>
#include "TlHelp32.h"
#ifdef _WIN32
std::wstring ProcInfoUtil<std::wstring>::GetProcName(unsigned int pid)
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