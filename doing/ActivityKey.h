#pragma once
#include <string>
struct ActivityKey
{
    std::wstring proc_name;
    std::wstring url;
    std::wstring window_text;
public:
    ActivityKey() {}
    void SetKey(const std::wstring& proc_name, const std::wstring url, const std::wstring window_text)
    {
        this->proc_name = proc_name;
        this->url = url;
        this->window_text = window_text;
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
    ActivityKey& operator=(const ActivityKey& another_key)
    {
        SetKey(another_key.proc_name, another_key.url, another_key.window_text);
        return *this;
    }
    ActivityKey(const ActivityKey& another_key)
    {
        SetKey(another_key.proc_name, another_key.url, another_key.window_text);
    }
    bool operator==(const ActivityKey& another_key)
    {
        return this->_key.compare(another_key.GetKey()) == 0;
    }
    bool operator!=(const ActivityKey& another_key)
    {
        return this->_key.compare(another_key.GetKey()) != 0;
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