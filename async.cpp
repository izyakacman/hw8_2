#include "async.h"
#include "command.h"

#include <sstream>
#include <thread>
#include <chrono>

namespace async {

handle_t connect(std::size_t bulk) 
{
    return new Command( bulk );
}

void receive(handle_t handle, const char *data, std::size_t size) 
{
    Command* ptr = reinterpret_cast<Command*>(handle);
    std::string s { data, size };
    std::istringstream stream{s};

    while (stream >> s)
    {
        if (ptr->ProcessCommand(s) == false) break;
    }
}

void disconnect(handle_t handle) 
{
    Command* ptr = reinterpret_cast<Command*>(handle);

    ptr->ProcessCommand(EndOfFileString);

    delete reinterpret_cast<Command*>(handle);
}

}
