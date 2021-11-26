#include <core/print.hpp>
#include <core/span.hpp>
#include "foundation/application.hpp"
#include <core/type_traits.hpp>
#include <core/ranges.hpp>
#include <list>
struct Object{
    void Method(){

    }
};

int StraitXMain(Span<const char *> args){

    std::list<char> a = {'a', 'b', 'c', 'd', 'e'};

    bool b = IsReverseRange<List<char>>::Value;

    Println("Forward: %", ForwardRange(a));
    Println("Reverse: %", ReverseRange(a));
    Println("Indexed: %", IndexedRange(a));


    return Application::Get().Run();
}