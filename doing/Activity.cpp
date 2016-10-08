#include "Activity.h"
#include <iostream>
#include <sstream>

Activity::Activity(const _doing_int64 dur,
    const _doing_int64 time_stamp,
    const std::wstring& machine_name,
    const ActivityKey& key):Activity(dur,time_stamp,machine_name,key,true)
{
}
Activity::Activity(const _doing_int64 dur, 
    const _doing_int64 time_stamp, 
    const std::wstring& machine_name, 
    const ActivityKey & key, 
    bool is_active) :
    _total_duration(dur),
    _time_stamp(time_stamp),
    _machine_name(machine_name),
    _proc_name(key.proc_name), //this shall not be empty and should not be empty
    _url(key.url.empty() ? L"null" : key.url),
    _window_title(key.window_text.empty() ? L"null" : key.window_text),
    _is_active(is_active)
{
}
std::wstring Activity::GetFormattedReport() const
{
    //agent_id@@@@@p_name@@@@@window_text@@@@@url@@@@@duration@@@@@datetime
    std::wstringstream wss;
    wss << _machine_name << L"@@@@@" << _proc_name << L"@@@@@" << _window_title
        << L"@@@@@" << _url << L"@@@@@" << _total_duration << L"@@@@@" << _time_stamp << L"@@@@@" << _is_active;
    return wss.str();

}
std::wstring Activity::GetKey() const
{
    return std::wstring();
}
Activity::~Activity()
{
    //clean up
}
//end of the file