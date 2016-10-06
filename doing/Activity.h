#pragma once
#include <string>
#include "Types.h"
#include <vector>
#include "ActivityKey.h"
class Activity
{
public:
    Activity(const _doing_int64 dur,
        const _doing_int64 time_stamp,
        const std::wstring machine_name,
        const ActivityKey& key);
    virtual ~Activity();
    std::wstring GetFormattedReport() const;
    std::wstring GetKey() const;
private:
    // long 4 bytes + long 4 bytes, the total duration that this activity has been running after last acitivity
    const _doing_int64 _total_duration;
    // UNIX EPOCH time in milliseconds, it's the time that the current activity is generated and next activity is about
    // to be recorded
    const _doing_int64 _time_stamp;
    //the machine name of where the process is running
    //TODO: create a init-handshake phase in websocket protocol so we don't need to add _machine_name in each activity
    const std::wstring _machine_name;
    //the process name
    const std::wstring _proc_name;
    //windows title
    std::wstring _window_title;
    //url 
    std::wstring _url;
    //const std::wstring _metric_key; //won't change after creation of this objec instance
};