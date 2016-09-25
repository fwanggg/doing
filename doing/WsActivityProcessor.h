#pragma once
#include <queue>
#include "Activity.h"
#include "ConcurrentQueue.h"
#include "ActivityProcessor.h"
class WsActivityProcessor : public ActivityProcessor
{
public:
    WsActivityProcessor(const std::wstring & ws_endpoint, ConcurrentQueue<Activity>& job_queue);
    virtual void RunThread();
    virtual void ProcessAcitivityQueue();
    virtual ~WsActivityProcessor();
private:
    std::wstring _ws_endpoint;
};