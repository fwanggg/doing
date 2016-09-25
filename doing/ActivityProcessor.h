#pragma once
#include "ConcurrentQueue.h"
#include "Activity.h"
class ActivityProcessor
{
public:
    virtual void ProcessAcitivityQueue() = 0;
    virtual void RunThread() = 0;
    virtual ~ActivityProcessor();
    ActivityProcessor(ConcurrentQueue<Activity>& _job_queue);
protected:
    ConcurrentQueue<Activity>&  m_job_queue;
};
