#include <vector>
#include <queue>
#include "Main.h"
#include <string>

bool generateBool()
{
    printf("generateBool called\n");
    return false;
}
void testStatic()
{
    static bool abc = generateBool();
}

std::wstring test_str(L"123");

/*
ex 2:

return a reference and use a reference is like using a pointer, it's possible to change internal data members of referenced
value. so to be very careful and use const whenver needed.

*/
std::wstring& getStr()
{
    return test_str;
}
void testReference()
{
    std::wstring& a = getStr();
    wprintf(L"%s", a.c_str()); // returns 123
    a = L"456"; // the copy-constructor of pointed(referenced) string is called

    wprintf(L"%s", a.c_str()); // returns 456
    wprintf(L"%s", test_str.c_str()); //returns 456
}


int main()
{
    
    //ex 1
    //vector copies element by using the element class's copy-constructor
    {
        std::vector<Foo> vec;
        for (int i = 0; i < 5; i++)
        {
            Foo f(i);
            vec.push_back(f);
        }
    }
    
    //ex 2, changing reference returned by a function won't actually change the data under the hood
    testReference();


    //ex 3, behavior of static local varibales
    for (int i = 0; i < 5; i++)
    {
        testStatic();
    }


    return -1;
}

//end of the file