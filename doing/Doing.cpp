#include "Doing.h"
#include <Windows.h>
#include <string>
#include "Win32Util.h"
#include <oleacc.h>
#include <iostream>
#include <atlbase.h>
#include <UIAutomationClient.h>
#include "StringUtil.h"
#include "Activity.h"
#include <ctime>
#include <thread>
#include <chrono>
Doing::Doing(ConcurrentQueue<Activity>& ptr_map) : _job_queue(ptr_map)
{
    ::CoInitialize(NULL);
    _machine_name = Win32Util<std::wstring>::GetMachineName();

}
void Doing::StartReportActivities() const
{
}
void Doing::Sample()
{
    ::CoInitialize(NULL);
    std::wstring last_url;
    std::wstring last_window_text;
    std::wstring last_proc_name;
    while (true)
    {
        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());
        if (_last_time != 0)
        {
            HWND hwnd = GetForegroundWindow();
            DWORD pid = 0;
            if (hwnd)
            {
                if (GetWindowThreadProcessId(hwnd, &pid))
                {
                    if (!(pid == GetCurrentProcessId()))
                    {
                        std::wstring proc_name = Win32Util<std::wstring>::GetProcName(pid);
                        //1. get key of the activity
                        std::wstring key = proc_name; //default key
                        std::wstring url;
                        std::wstring window_title = Win32Util<std::wstring>::GetWindowTitleByHandle(hwnd);
                        if (proc_name.compare(L"chrome.exe") == 0)
                        {
                            url = ReportUrlByHandle(hwnd);
                            key.append(L"@@@@@@");
                            key.append(url);
                            key.append(L"@@@@@@");//url won't have this 
                            key.append(window_title);
                        }
                        else
                        {
                            key.append(L"@@@@@@");
                            key.append(window_title);
                        }

                        if (_last_metric_key.empty()) // first time
                        {
                            _last_metric_key = key;
                            last_url = url;
                            last_window_text = window_title;
                            last_proc_name = proc_name;
                        }
                        else
                        {
                            //if this is still the same activity, increase the duration
                            _acitive_duration += ms.count() - _last_time;
                            if (key.compare(_last_metric_key) != 0) //only when activity changes do we generate a activity
                            {
                                Activity activity(_acitive_duration,
                                    ms.count(),
                                    _machine_name,
                                    last_proc_name);
                                if (!url.empty())
                                {
                                    activity.SetUrl(last_url);
                                }
                                if (!window_title.empty())
                                {
                                    activity.SetTile(last_window_text);
                                }
                                if (_job_queue.GetSize() < 1000)
                                {
                                    _job_queue.Push(activity);
                                }
                                else
                                {
                                    //fucked
                                }

                                //update the global single threaded variables
                                _acitive_duration = 0;
                                _last_metric_key = key;
                                last_url = url;
                                last_window_text = window_title;
                                last_proc_name = proc_name;
                            }
                            
                        }
                    }
                }
                else
                {
                    wprintf(L"GetWindowThreadProcessId failed errcode = %d", GetLastError());
                }
            }
        }


        //always bumps this, we are increasing this in a 500 ms 
        {
            _last_time = ms.count();
        }
        
        Sleep(500);
    }
}
void Doing::Monitor()
{
    std::thread t_sampling(&Doing::Sample,this);
    t_sampling.detach();
}
Doing::~Doing()
{
    ::CoUninitialize();
}
std::wstring Doing::ReportUrlByHandle(const HWND hwnd)
{
    CComPtr<IUIAutomation> pAutomation = NULL;
    HRESULT hr = pAutomation.CoCreateInstance(CLSID_CUIAutomation, NULL, CLSCTX_INPROC_SERVER);
    CComPtr<IUIAutomationElement> pElement = NULL;
    std::wstring val_std_str;
    if (SUCCEEDED(hr))
    {
        hr = pAutomation->ElementFromHandle(hwnd, &pElement);
        if (pElement)
        {
            CComPtr<IUIAutomationCondition> pCondition = NULL;
            IUIAutomationElement* pFound = NULL;  // don't use smart pointer because we want non RAII control over this pointer
            BSTR urlControlName = SysAllocString(L"Address and search bar");
            if (urlControlName)
            {
                VARIANT varProp;
                varProp.vt = VT_BSTR;
                varProp.bstrVal = urlControlName;
                pAutomation->CreatePropertyCondition(UIA_NamePropertyId, varProp, &pCondition);
                if (pCondition)
                {
                    pElement->FindFirst(TreeScope_Descendants, pCondition, &pFound);
                    if (pFound)
                    {
                        BSTR val = NULL;
                        BSTR name = NULL;
                        CComPtr<IUIAutomationValuePattern> ptr_val_pattern = NULL;
                        pFound->get_CurrentName(&name);
                        pFound->GetCurrentPatternAs(UIA_ValuePatternId, __uuidof(IUIAutomationValuePattern), (void**)&ptr_val_pattern);
                        ptr_val_pattern->get_CurrentValue(&val);
                        val_std_str = StringUtilBase::BSTR2StdString(val);
                        //std::wstring name_std_str = StringUtilBase<std::wstring>::BSTR2StdString(name);
                        pFound->Release();
                    }
                }

                SysFreeString(urlControlName);
            }
        }
    }
    return val_std_str;
}
//end of the file