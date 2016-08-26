#include "Activity.h"
#include <iostream>
#include <sstream>

Activity::Activity()
    :_metric_key(L"Unknown Activity"){}

Activity::Activity(const std::wstring & metric_key, unsigned long long dur):
    _metric_key(metric_key),
    _total_duration(dur)
{}
std::wstring Activity::GetFormattedReport()
{
    std::wstringstream wss;
    wss << L"[" << _metric_key << L"]" << L":" << _total_duration/1000 << std::endl;
    return wss.str();

}
Activity::~Activity()
{
    //clean up
}
//end of the file