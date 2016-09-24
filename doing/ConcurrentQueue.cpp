#include "ConcurrentQueue.h"
#include "Activity.h"
template<typename K> K ConcurrentQueue<K>::GetAndPop()
{
    std::lock_guard<std::mutex> lock(_queue_lock);
    K elem = _queue.front();
    _queue.pop();
    return elem;
}
template<typename K> size_t ConcurrentQueue<K>::GetSize()
{
    std::lock_guard<std::mutex> lock(_queue_lock);
    return _queue.size();
}
template<typename K> void ConcurrentQueue<K>::Push(K& val)
{
    std::lock_guard<std::mutex> lock(_queue_lock);
    _queue.push(val);
}

//In order for the compiler to generate the code, it must see both the template definition (not just declaration) 
//and the specific types/whatever used to “fill in” the template.
template class ConcurrentQueue<Activity>;

//template<typename K, typename V> std::mutex ConcurrentMap<K,V>::_map_lock;
//template<typename K, typename V> std::map<K, V> ConcurrentMap<K,V>::_map;
//end of the line