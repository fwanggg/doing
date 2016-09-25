#pragma once
#include "ActivityProcessor.h"
class ConsoleActivityProcessor : public ActivityProcessor
{
public:
    ConsoleActivityProcessor(ConcurrentQueue<Activity>& job_queue);
    virtual void RunThread();
    virtual void ProcessAcitivityQueue();
    virtual ~ConsoleActivityProcessor();
};