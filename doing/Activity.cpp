#include "Activity.h"
#include <iostream>
#include <sstream>

void Activity::SetUrl(std::wstring url)
{
    _url = url;
}
void Activity::SetTile(std::wstring title)
{
    _window_title = title;
}
Activity::Activity(const _doing_int64 dur,
    const _doing_int64 time_stamp,
    const std::wstring machine_name,
    const std::wstring proc_name):
    _total_duration(dur),
    _time_stamp(time_stamp),
    _machine_name(machine_name),
    _proc_name(proc_name),
    _url(L"null"),
    _window_title(L"null")
{}
std::wstring Activity::GetFormattedReport() const
{
    std::wstringstream wss;
    wss << _machine_name << L"@@@@@" << _total_duration << L"@@@@@" << _time_stamp
        << L"@@@@@" << _url << L"@@@@@" << _window_title << L"@@@@@" << _proc_name << std::endl;
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