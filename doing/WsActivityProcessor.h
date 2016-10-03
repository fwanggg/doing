#pragma once
#include <queue>
#include <memory>
#include "Activity.h"
#include "ConcurrentQueue.h"
#include "ActivityProcessor.h"
#include "easywsclient.hpp"
using easywsclient::WebSocket;
class WsActivityProcessor : public ActivityProcessor
{
public:
    WsActivityProcessor(const std::wstring & ws_endpoint, ConcurrentQueue<Activity>& job_queue);
    virtual void RunThread();
    virtual void ProcessAcitivityQueue();
    virtual ~WsActivityProcessor();
private:
    std::wstring _ws_endpoint;
    std::string _ws_endpoint_str;
    //don't initialize websocket in constructor, because i want this object to be created no matter what
    //then handling failure in the ProcessAcitivityQueue method
    std::shared_ptr<WebSocket> ws;
    void RetryAfter(int ms);
};