#pragma once
#include <queue>
#include <mutex>
//dont inhetit std::map, it has no virtual destructor
//std::map* ptr = new ConcurrentMap();
//delete ptr;
//above code results in undefined behavior
#define DEFAULT_QUEUE_SIZE 1000
template<typename K> class ConcurrentQueue
{
private:
    std::mutex _queue_lock;
    std::queue<K> _queue;
    unsigned int _size = DEFAULT_QUEUE_SIZE; //default to 1000
public:
    //first call front() then poll()
    ConcurrentQueue(unsigned int size) :_size(size) {}
    K GetAndPop();
    void Push(K& val);
    size_t GetSize();
};
