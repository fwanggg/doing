#include "Activity.h"
#include <iostream>
#include <sstream>

Activity::Activity( 
    const _doing_int64 start_time, 
    const std::wstring& machine_name, 
    const ActivityKey & key) :
    _start_time(start_time),
    _machine_name(machine_name),
    _proc_name(key.proc_name), //this shall not be empty and should not be empty
    _url(key.url.empty() ? L"null" : key.url),
    _window_title(key.window_text.empty() ? L"null" : key.window_text)
{
}
Activity::Activity(const Activity & another) : _machine_name(another._machine_name),
_start_time(another._start_time),
_proc_name(another._proc_name),
_url(another._url),
_window_title(another._window_title),
_is_active(another._is_active),
_total_duration(another._total_duration)
{
}
Activity::Activity():_start_time(0)
{
    //place holder
}
std::wstring Activity::GetFormattedReport() const
{
    //agent_id@@@@@p_name@@@@@window_text@@@@@url@@@@@duration@@@@@datetime
    std::wstringstream wss;
    wss << _machine_name << L"@@@@@" << _proc_name << L"@@@@@" << _window_title
        << L"@@@@@" << _url << L"@@@@@" << _total_duration << L"@@@@@" << _start_time << L"@@@@@" << _is_active;
    return wss.str();

}
std::wstring Activity::GetKey() const
{
    return std::wstring();
}
void Activity::BumpDuration(_doing_int64 duration_delta)
{
    _total_duration += duration_delta;
}
void Activity::SetIsActive(bool is_active)
{
    _is_active = is_active;
}
Activity & Activity::operator=(const Activity & rhs)
{
    // TODO: insert return statement here
    if (this != &rhs)
    {
        this->_is_active = rhs._is_active;
        this->_machine_name = rhs._machine_name;
        this->_proc_name = rhs._proc_name;
        this->_start_time = rhs._start_time;
        this->_total_duration = rhs._total_duration;
        this->_url = rhs._url;
        this->_window_title = rhs._window_title;
    }
    return *this;
}
Activity::~Activity()
{
    //clean up
}
//end of the file