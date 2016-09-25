#pragma once
#include <windows.h>
#include <string>

////WNDENUMPROC
//BOOL CALLBACK EnumChildWindowsProcCallBack(
//    _In_ HWND   hwnd,
//    _In_ LPARAM lParam
//);
//
////WNDENUMPROC
//BOOL CALLBACK EnumWindowsProcCallBack(
//    _In_ HWND   hwnd,
//    _In_ LPARAM lParam
//);
//void CALLBACK ChromeURLEditBoxEventCallback(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);

static std::wstring g_remote_address;
static bool g_to_console = false;
//end of the file
