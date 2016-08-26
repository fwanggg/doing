#pragma once
#include <windows.h>
#include <map>
#include "Activity.h"
#include <mutex>
#include <queue>

class Doing
{
public:
    typedef std::map<std::wstring, Activity*> ProcActivityMap;
    typedef std::pair<std::wstring, Activity*> ProcActivityPair;
    void Monitor() const;
    void StartReportActivities() const;
    Doing();
    ~Doing();
    static std::queue<Activity> _activity_job_queue;
private:
    static std::wstring _last_metric_key;
    static unsigned long long _acitive_duration;
    static std::mutex _job_queue_lock;
    static Activity* s_current_metric;
    static void CALLBACK WinEventProcCallback(
        HWINEVENTHOOK hWinEventHook,
        DWORD         event,
        HWND          hwnd,
        LONG          idObject,
        LONG          idChild,
        DWORD         dwEventThread,
        DWORD         dwmsEventTime
    );
    static void Sample();
    static std::wstring ReportUrlByHandle(const HWND hwnd);
    //static ProcActivityMap s_metric_map;
    
    static unsigned long long _last_time;
};
