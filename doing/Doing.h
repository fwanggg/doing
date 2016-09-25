#pragma once
#include <windows.h>
#include <map>
#include "Activity.h"
#include <mutex>
#include <queue>
#include "ConcurrentQueue.h"
#include <memory>

class Doing
{
public:
    typedef std::map<std::wstring, Activity*> ProcActivityMap;
    typedef std::pair<std::wstring, Activity*> ProcActivityPair;
    void Monitor();
    void StartReportActivities() const;
    Doing(ConcurrentQueue<Activity>& ptr_queue);
    ~Doing();
private:
    Doing();
    ConcurrentQueue<Activity>& _job_queue;
    std::wstring _last_metric_key;
    unsigned long long _acitive_duration;
    void Sample();
    std::wstring ReportUrlByHandle(const HWND hwnd);
    //static ProcActivityMap s_metric_map;
    
    unsigned long long _last_time;
};
