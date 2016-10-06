#pragma once
#include <iostream>
class Foo
{
public:
    Foo(int id):_id(id) {
        std::cout << "Foo " << id << " constructor is called. " << std::endl;
    }
    Foo(Foo&& f)
    {
        
        _id = f._id;
        std::cout << "Foo " << _id << " move constructor is called. " << std::endl;
    }// force a move ctor anyway
    //copy constructor
    Foo(const Foo& another_foo) {
        this->_id = another_foo._id;
        std::cout << "Foo " << another_foo._id << " copy constructor is called. " << std::endl;
    }
    ~Foo() {
        std::cout << "Foo " << _id << " destructor is called. " << std::endl;
    };
    int _id;
};
