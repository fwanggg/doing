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
    ActivityKey cur_activity_key;
    bool prev_is_active = true;
    while (true)
    {
        ActivityKey next_activity_key;
        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());
        unsigned long long start_time = 0; // 0 can be used to verify if start_time has been inited or not
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
                        next_activity_key.proc_name = Win32Util<std::wstring>::GetProcName(pid);;
                        //1. get key of the activity
                        std::wstring url;
                        next_activity_key.window_text = Win32Util<std::wstring>::GetWindowTitleByHandle(hwnd);
                        if (next_activity_key.proc_name.compare(L"chrome.exe") == 0)
                        {
                            next_activity_key.url = ReportUrlByHandle(hwnd);
                        }

                        //creates key internally
                        //as of this step, the internal data members should all be set
                        next_activity_key.GenerateKey();

                        if (cur_activity_key.IsEmpty()) // first time
                        {
                            cur_activity_key = next_activity_key; //copy constructor
                            start_time = ms.count();//init start time
                        }
                        else
                        {
                            _acitive_duration += ms.count() - _last_time;//if this is still the same activity, increase the duration
                            bool cur_is_active = prev_is_active; 
                            try
                            {
                                cur_is_active = (GetTickCount() - GetLastInputTime()) < 120000;
                                //debug purpose
                                if (cur_is_active != prev_is_active)
                                {
                                    wprintf(L"activity %s activeness changed from %d to %d\n",next_activity_key.GetKey().c_str(), prev_is_active,cur_is_active);
                                    //SimpleLogger::GetInstance().WriteLogMessage(LOGGER_LEVEL::INFO_LVL,);
                                }
                            }
                            catch (const std::exception& e)
                            {
                                std::string s = e.what();
                                //ya lazy ass. handle i18n multibyte to wide char when it breaks :P
                                SimpleLogger::GetInstance().WriteLogMessage(LOGGER_LEVEL::ERROR_LVL, std::wstring(s.begin(), s.end()));
                            }

                            //When Activity itself has changed
                            if (next_activity_key != cur_activity_key)                     
                            {
                                Activity activity(_acitive_duration,
                                    start_time,
                                    _machine_name,
                                    cur_activity_key,
                                    cur_is_active);
                                _job_queue.Push(activity);                                
                                cur_activity_key = next_activity_key;

                                //previous recording of this activity is done
                                //now reset all bookkeeping local variables
                                _acitive_duration = 0;
                                prev_is_active = true;
                                start_time = ms.count(); //reset start time
                            }
                            else if (cur_is_active != prev_is_active)
                            {
                                Activity activity(_acitive_duration,
                                    start_time,
                                    _machine_name,
                                    cur_activity_key,
                                    //cur_is_active actually describes current activity's activeness
                                    //prev_is_active describes the *already queued*, last acitivity
                                    cur_is_active);
                                _job_queue.Push(activity);
                                prev_is_active = cur_is_active;
                                cur_activity_key = next_activity_key;
                                //previous recording of this activity is done
                                //now reset all bookkeeping local variables
                                _acitive_duration = 0;
                                start_time = ms.count(); //reset start time
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