#pragma once
#include <queue>
#include "Activity.h"
class ActivityProcessor
{
public:
    ActivityProcessor(std::queue<Activity> & job_queue);
    void run();
    void ProcessAcitivityQueue();
private:
    std::queue<Activity> & _job_queue;
};