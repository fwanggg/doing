#pragma once
#include <string>
#include "Types.h"
#include <vector>
#include "ActivityKey.h"
class Activity
{
public:
    Activity(const _doing_int64 time_stamp,
        const std::wstring& machine_name,
        const ActivityKey& key);
    Activity(const Activity& another);
    Activity();
    virtual ~Activity();
    std::wstring GetFormattedReport() const;
    std::wstring GetKey() const;
    void BumpDuration(_doing_int64 duration_delta);
    void SetIsActive(bool is_active);
    Activity& operator=(const Activity& rhs);
private:
    // long 4 bytes + long 4 bytes, the total duration that this activity has been running after last acitivity
    _doing_int64 _total_duration = 0;
    // UNIX EPOCH time in milliseconds, it's the time that the current activity is generated and next activity is about
    // to be recorded
    _doing_int64 _start_time;
    //the machine name of where the process is running
    //TODO: create a init-handshake phase in websocket protocol so we don't need to add _machine_name in each activity
    std::wstring _machine_name;
    //the process name
    std::wstring _proc_name;
    //if this is an active activity or not
    bool _is_active = true;
    //windows title
    std::wstring _window_title;
    //url 
    std::wstring _url;
    //const std::wstring _metric_key; //won't change after creation of this objec instance
};