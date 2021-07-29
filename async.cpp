#include "async.h"
#include "commands_processor.h"

#include <sstream>
#include <thread>
#include <chrono>

namespace async {

handle_t connect(std::size_t bulk) 
{
    return new CommandsProcessor( bulk );
}

void receive(handle_t handle, const char *data, std::size_t size) 
{
    CommandsProcessor* ptr = reinterpret_cast<CommandsProcessor*>(handle);
    std::string s { data, size };
    std::istringstream stream{s};

    while (stream >> s)
    {
        if (ptr->ProcessCommand(s) == false) break;
    }
}

void disconnect(handle_t handle) 
{
    CommandsProcessor* ptr = reinterpret_cast<CommandsProcessor*>(handle);

    ptr->ProcessCommand(EndOfFileString);

    delete reinterpret_cast<CommandsProcessor*>(handle);
}

}
