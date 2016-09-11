#include "main.h"
#include <stdio.h>
#include "ProcInfoUtil.h"
#include <oleacc.h>
#include <iostream>
#include <atlbase.h>
#include <UIAutomationClient.h>
#include "StringUtil.h"
#include "Doing.h"
#include "ActivityProcessor.h"

int main(int argc, char* argv[])
{
    if (argc == 0 || argc == 1)  //screwed up or just a executable
    {
        g_remote_address = L"ws://localhost:8083/AS/websocket/activity";
    }
    else
    {
        std::string arg_one_str = argv[1];
        std::wstring arg_one_wstr(arg_one_str.begin(),arg_one_str.end());
        g_remote_address = L"ws://";
        g_remote_address.append(arg_one_wstr);
        g_remote_address.append(L"/AS/websocket/activity");
    }
    std::wcout << L"remote address set to " << g_remote_address << std::endl;
    //TODO: refactor out _activity_job_queue. must extract and make sure job_queue is accessed thread-safely
    ActivityProcessor ap(g_remote_address,Doing::_activity_job_queue);
    ap.run();



    Doing doing;
    doing.Monitor();
}

/*void CALLBACK ChromeURLEditBoxEventCallback(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {
    DWORD pid = 0;
    {
        //GetWindowText(hwnd, buf, 1024);
        {
            {
                ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
                IAccessible* pAcc = NULL;
                VARIANT varChild;
                std::wcout << L"event raised : " << std::hex << event << std::endl;
                HRESULT hr = AccessibleObjectFromEvent(hwnd, idObject, idChild, &pAcc, &varChild);
                if ((hr == S_OK) && (pAcc != NULL)) {
                    BSTR bstrName, bstrValue;
                    pAcc->get_accValue(varChild, &bstrValue);
                    pAcc->get_accName(varChild, &bstrName);

                    wchar_t className[50];
                    GetClassName(hwnd, className, 50);
                    if (bstrValue != NULL && bstrName != NULL && (lstrcmp(className, L"Chrome_WidgetWin_1") == 0) && (wcscmp(bstrName, L"Address and search bar") == 0)) {
                        wprintf(L"URL change: %ls\n", bstrValue);
                    }
                    pAcc->Release();
                }
                ::CoUninitialize();
            }
        }
    }

}
//WNDENUMPROC
BOOL CALLBACK EnumWindowsProcCallBack(
    _In_ HWND   hwnd,
    _In_ LPARAM lParam
)
{
    wchar_t buf[1024] = { 0 };
    wchar_t buf_text[1024] = { 0 };
    GetClassName(hwnd, buf, 1024);
    GetWindowText(hwnd, buf_text, 1024);
    wprintf(L"class name %s, text is [%s] \n", buf, buf_text);
    EnumChildWindows(hwnd, EnumChildWindowsProcCallBack, NULL);
    return TRUE;
}
//WNDENUMPROC
BOOL CALLBACK EnumChildWindowsProcCallBack(
    _In_ HWND   hwnd,
    _In_ LPARAM lParam
)
{
    wchar_t buf[1024] = { 0 };
    wchar_t buf_text[1024] = { 0 };
    GetClassName(hwnd, buf, 1024);
    GetWindowText(hwnd, buf_text, 1024);
    if (lstrlen(buf_text) != 0)
    {
        wprintf(L"class name %s, text is [%s] \n", buf, buf_text);
    }
    std::wstring clsName = L"Chrome_OmniboxView";
    if (clsName.compare(buf) == 0)
    {
        wprintf(L"found it!!");
    }
    return TRUE;
}*/

//end of the file