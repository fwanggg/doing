#pragma once
#include <string>
#include <vector>
class Activity
{
public:
    Activity(const std::wstring& key, unsigned long long dur);
    Activity();
    virtual ~Activity();
    std::wstring GetFormattedReport();
private:
    // long 4 bytes + long 4 bytes
    unsigned long long _total_duration;
    const std::wstring _metric_key; //won't change after creation of this objec instance
};