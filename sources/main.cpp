#include <core/print.hpp>
#include <core/span.hpp>
#include "foundation/application.hpp"

int StraitXMain(Span<const char *> args){
    return Application::Get().Run();
}