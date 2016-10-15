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
#include "ActivityKey.h"
#include "SimpleLogger.h"
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
    ActivityKey prev_activity_key;
    bool prev_is_active = true;
    Activity activity;

    while (true)
    {
        //cannot reuse.. suchs
        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());
        if (_last_time != 0)
        {
            ActivityKey cur_activity_key;
            //fill the activity key if we need to
            FillActivityKey(cur_activity_key);
            bool still_active = cur_activity_key.is_active;
            //activity key has been filled in. This activity is worth looking at furthur
            if (!cur_activity_key.IsEmpty())
            {
                if (prev_activity_key.IsEmpty()) // first time
                {
                    prev_activity_key = cur_activity_key; // init prev_activity_key using copy ctor
                    activity = Activity(ms.count(), //init activity with valid data for the first time
                        _machine_name,
                        cur_activity_key);
                }
                else
                {
                    activity.BumpDuration(ms.count() - _last_time);
                    //When Activity itself has changed
                    if (cur_activity_key != prev_activity_key)
                    {
                        //is_active is a after-fact calculation
                        //it's extracted from current activity but need to be updated to prev activity
                        activity.SetIsActive(still_active);

                        //for the sake of clean result don't process doing.exe itself
                        //1. put the activity that was just finished monitoring on to the queue for furthur processing
                        _job_queue.Push(activity);
                        //2. set activity to the new activity
                        activity = Activity(ms.count(),
                            _machine_name,
                            cur_activity_key);
                        prev_activity_key = cur_activity_key;
                    }
                }
            }
        }
        _last_time = ms.count();

        Sleep(500);
    }
}
void Doing::Monitor()
{
    std::thread t_sampling(&Doing::Sample, this);
    t_sampling.detach();
}
Doing::~Doing()
{
    ::CoUninitialize();
}
void Doing::FillActivityKey(ActivityKey & key)
{
    HWND hwnd = GetForegroundWindow();
    if (hwnd)
    {
        DWORD pid = 0;
        if (GetWindowThreadProcessId(hwnd, &pid))
        {
            if (!(pid == GetCurrentProcessId()))
            {
                key.proc_name = Win32Util<std::wstring>::GetProcName(pid);;
                //1. get key of the activity
                std::wstring url;
                key.window_text = Win32Util<std::wstring>::GetWindowTitleByHandle(hwnd);
                if (key.proc_name.compare(L"chrome.exe") == 0)
                {
                    key.url = ReportUrlByHandle(hwnd);
                }
                key.is_active = (GetTickCount() - GetLastInputTime()) < 120000;
                //creates key internally
                //as of this step, the internal data members should all be set
                key.GenerateKey();
            }
        }
    }
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
/*
desc:
    Retrieves the number of milliseconds that have elapsed since the system was started, up to 49.7 day
    since last user input

ret:
    last user input time in milis

exception:
    throw std::exception when GetLastInputInfo fails.
    MSDN does not mention what ERROR CODE to expect when failing though.

*/
unsigned long long Doing::GetLastInputTime()
{
#ifdef _WIN32
    LASTINPUTINFO lii;
    //The size of the structure, in bytes. This member must be set to sizeof(LASTINPUTINFO).
    //It's a common way for versioning in Windows API.
    lii.cbSize = sizeof(LASTINPUTINFO);
    if (!GetLastInputInfo(&lii))
    {
        throw std::exception("GetLastInputInfo failed!");
    }
    return lii.dwTime;
#else   
    return 0;
#endif
}
//end of the file