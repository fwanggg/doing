#include "main.h"
#include <stdio.h>
#include "Win32Util.h"
#include <oleacc.h>
#include <iostream>
#include <atlbase.h>
#include <UIAutomationClient.h>
#include "StringUtil.h"
#include "Doing.h"
#include "WsActivityProcessor.h"
#include "ConsoleActivityProcessor.h"
#include <memory>

void PrintHelp() 
{
    printf("Args not recognized.\n\n\n");
    printf("Allowed arguments:\n");
    printf("\t-console\t\t\toutput acitivies to console.\n");
    printf("\t-remote URL\t\t\testablish websocket connection with URL provided and stream the activities to remote server.\n");
}

int main(int argc, char* argv[])
{
    if (argc == 0 || argc == 1)  //screwed up or just a executable
    {
        //bad commandline
        PrintHelp();
    }
    else
    {
        bool shouldStart = false;
        std::string arg_opt = argv[1];
        if (arg_opt == "-console" && argc == 2)
        {
            //write to console
            g_to_console = true;
            shouldStart = true;
        }
        else if(arg_opt == "-remote" && argc == 3)
        {
            //talk to remote server
            //now we expect server url
            std::string arg_one_str = argv[2];
            std::wstring arg_one_wstr(arg_one_str.begin(), arg_one_str.end());
            //g_remote_address = L"ws://";
            g_remote_address.append(arg_one_wstr);
            //g_remote_address.append(L"/AS/websocket/activity");
            std::wcout << L"remote address set to " << g_remote_address << std::endl;
            shouldStart = true;
        }
        if (shouldStart)
        {
            //create the job queue
            ConcurrentQueue<Activity> job_queue;
            ActivityProcessor* ptr_ap = nullptr; // lifetime is same as the process no need to delete atm
            if (g_to_console)
            {
                ptr_ap = new ConsoleActivityProcessor(job_queue);
            }
            else
            {
                ptr_ap = new WsActivityProcessor(g_remote_address, job_queue);
            }
            ptr_ap->RunThread();
            //start producer thread and pass in job queue
            Doing doing(job_queue);
            doing.Monitor();//and detach
#ifdef _WIN32
            Sleep(INFINITE);
#else
#endif
        }
        else
        {
            PrintHelp();
        }
    }
    

    
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