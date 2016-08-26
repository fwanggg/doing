#pragma once
#include <windows.h>
#include <string>

//WNDENUMPROC
BOOL CALLBACK EnumChildWindowsProcCallBack(
    _In_ HWND   hwnd,
    _In_ LPARAM lParam
);

//WNDENUMPROC
BOOL CALLBACK EnumWindowsProcCallBack(
    _In_ HWND   hwnd,
    _In_ LPARAM lParam
);
void CALLBACK ChromeURLEditBoxEventCallback(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);


//end of the file
