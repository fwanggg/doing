#pragma once
#include <queue>
#include <mutex>
//dont inhetit std::map, it has no virtual destructor
//std::map* ptr = new ConcurrentMap();
//delete ptr;
//above code results in undefined behavior
template<typename K> class ConcurrentQueue
{
private:
    std::mutex _queue_lock;
    std::queue<K> _queue;
public:
    //first call front() then poll()
    K GetAndPop();
    void Push(K& val);
    size_t GetSize();
};
