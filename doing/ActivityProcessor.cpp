#include "ActivityProcessor.h"
#include <Windows.h>
#include <thread>
#include <iostream>
#include "easywsclient.hpp"
#include "StringUtil.h"
using easywsclient::WebSocket;
ActivityProcessor::ActivityProcessor(const std::wstring & ws_endpoint,std::queue<Activity> & job_queue)
    :_job_queue(job_queue),
    _ws_endpoint(ws_endpoint)
{}
void ActivityProcessor::ProcessAcitivityQueue()
{
    //conversion assumes the ws endpoint contains only ASCII characters
    std::string ws_endpoint_str(_ws_endpoint.begin(),_ws_endpoint.end());
    //anything bad happens, from_url returns NULL
    std::unique_ptr<WebSocket> ws(WebSocket::from_url(ws_endpoint_str));
    bool connOK = false;
    while (true)
    {
        connOK = ws && ws->getReadyState() != WebSocket::CLOSED;
        while (!_job_queue.empty())
        {
            //=================================
            //doing real processing work
            //=================================
            //use easywsclient to talk to server via ws api
            if (connOK)
            {
                Activity activity = _job_queue.front();
                std::wstring wc_str = activity.GetFormattedReport();
                std::string mb_str(wc_str.begin(),wc_str.end()); //TODO: handle i18n later
                ws->send(mb_str);   //according to easywsclient's doc, this only puts message into a buffer
                ws->poll(1000);     //while this actually does the real send I/O
                                    //for some reason, stl queue pop just don't return you the element
                _job_queue.pop();
            }
        }
        Sleep(500);
        //ws disconnect for some reason. Now retry.
        if (!connOK)
        {
            if (ws)
            {
                //to avoid leak any resource, release ws now
                //TODO: read its source code and determine if this is necessary
                ws->close();
            }
            ws.reset(WebSocket::from_url(ws_endpoint_str));
            //retry connection every 10 seconds
            Sleep(10000);
        }
    }
}

void ActivityProcessor::run()
{
#ifdef _WIN32
    INT rc;
    WSADATA wsaData;

    rc = WSAStartup(MAKEWORD(2, 2), &wsaData); // this thread runs forever.
    if (rc) {
        printf("WSAStartup Failed.\n");
        return;
    }
#endif
    //~thread(): Destroys the thread object. If *this still has an associated running thread (i.e. joinable() == true), 
    //std::terminate() is called.
    std::thread t(&ActivityProcessor::ProcessAcitivityQueue,this);
    t.detach();
}
//end of the file