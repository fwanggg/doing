#include "ActivityProcessor.h"
#include <Windows.h>
#include <thread>
#include <iostream>
ActivityProcessor::ActivityProcessor(std::queue<Activity> & job_queue):_job_queue(job_queue)
{}
void ActivityProcessor::ProcessAcitivityQueue()
{
    while (true)
    {
        while (!_job_queue.empty())
        {
            Activity activity = _job_queue.front();
            //=================================
            //doing real processing work
            //=================================

            //sendToAws(activity);
            std::wcout << activity.GetFormattedReport() << std::endl;
            //for some reason, stl queue pop just don't return you the element
            _job_queue.pop();
        }
        Sleep(500);
    }
}

void ActivityProcessor::run()
{
    //~thread(): Destroys the thread object. If *this still has an associated running thread (i.e. joinable() == true), 
    //std::terminate() is called.
    std::thread t(&ActivityProcessor::ProcessAcitivityQueue,this);
    t.detach();
}
//end of the file