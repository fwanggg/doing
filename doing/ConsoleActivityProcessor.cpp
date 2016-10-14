#include "ConsoleActivityProcessor.h"
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif
ConsoleActivityProcessor::ConsoleActivityProcessor(ConcurrentQueue<Activity>& job_queue):
    ActivityProcessor(job_queue)
{
}
void ConsoleActivityProcessor::RunThread()
{
    //~thread(): Destroys the thread object. If *this still has an associated running thread (i.e. joinable() == true), 
    //std::terminate() is called.
    std::thread t(&ConsoleActivityProcessor::ProcessAcitivityQueue, this);
    t.detach();
}
void ConsoleActivityProcessor::ProcessAcitivityQueue()
{
    while (true)
    {
        while (m_job_queue.GetSize() != 0)
        {
            Activity activity = m_job_queue.GetAndPop();
            std::wstring wc_str = activity.GetFormattedReport();
            std::wcout << wc_str << std::endl << std::flush;
        }
        Sleep(500);
    }
}
ConsoleActivityProcessor::~ConsoleActivityProcessor()
{
    //do nothing atm
}