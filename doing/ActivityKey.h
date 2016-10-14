#pragma once
#include <string>
struct ActivityKey
{
    std::wstring proc_name;
    std::wstring url;
    std::wstring window_text;
    bool is_active;
public:
    ActivityKey() {}
    void SetKey(const std::wstring& proc_name, const std::wstring url, const std::wstring window_text, bool is_active)
    {
        this->proc_name = proc_name;
        this->url = url;
        this->window_text = window_text;
        this->is_active = is_active;;
        GenerateKey();
    }
    void GenerateKey()
    {
        _key = proc_name;
        _key.append(L"@@@@@");
        _key.append(this->url);
        _key.append(L"@@@@@");
        _key.append(this->window_text);
    }
    /*
    Unless you define a copy ctor, a default will be generated (if needed).
    Unless you define an operator=, a default will be generated (if needed).
    They do not use each other, and you can change them independently.
    */
    ActivityKey& operator=(const ActivityKey& rhs)
    {
        SetKey(rhs.proc_name, rhs.url, rhs.window_text, rhs.is_active);
        return *this;
    }
    ActivityKey(const ActivityKey& another_key)
    {
        SetKey(another_key.proc_name, another_key.url, another_key.window_text, another_key.is_active);
    }
    bool operator==(const ActivityKey& rhs)
    {
        return this->_key.compare(rhs.GetKey()) == 0 
            && rhs.is_active == is_active;
    }
    bool operator!=(const ActivityKey& rhs)
    {
        return !(this->_key.compare(rhs.GetKey()) == 0
            && rhs.is_active == is_active);
    }
    bool IsEmpty() const
    {
        return _key.empty();
    }
    const std::wstring& GetKey() const
    {
        return _key;
    }
private:
    std::wstring _key;

};