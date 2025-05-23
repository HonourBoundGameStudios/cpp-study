#include <iostream>

#include "StreamFlix.h"
#include "HTTPRequest/include/HTTPRequest.hpp"

int main()
{
    StreamFlix::Run();
    StreamFlix::Shutdown();
}
