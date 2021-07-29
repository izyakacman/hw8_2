#include "commands_processor.h"
#include "writer.h"

#include <chrono>

using namespace std;

namespace{

void CoutWriterThr(queue<string>& stringQueue, mutex& threadMutex, bool& bStopFlag)
{
	CoutWriter writer;

	while (true)
	{
		lock_guard<std::mutex> guard(threadMutex);

		if (!stringQueue.empty())
		{
			writer.Print(move(stringQueue.front()));
			stringQueue.pop();
		}
		else
		{
			if (!bStopFlag) break;
		}

	} // while (bStopFlag)
}

void FileWriterThr(std::queue<std::pair<std::string, long long>>& stringQueue, std::mutex& threadMutex, bool& bStopFlag, int postfix)
{
	FileWriter writer;

	while (true)
	{
		lock_guard<std::mutex> guard(threadMutex);

		if (!stringQueue.empty())
		{
			auto p = move(stringQueue.front());
			writer.Print(p.second, postfix, p.first);
			stringQueue.pop();
		}
		else
		{
			if (!bStopFlag) break;
		}

	} // while (bStopFlag)
}

} // namespace

CommandsProcessor::CommandsProcessor(size_t count) : count_{ count }
{
	handler_ = ICommandHandlerPtr{ new StaticCommandHandler(count_) };

	cout_thr = thread(CoutWriterThr, ref(stringsQueueCout), ref(ThreadCoutMutex), ref(stop_flag_));
	file_thr1 = thread(FileWriterThr, ref(stringsQueueFile), ref(ThreadFileMutex), ref(stop_flag_), 1);
	file_thr2 = thread(FileWriterThr, ref(stringsQueueFile), ref(ThreadFileMutex), ref(stop_flag_), 2);
}

CommandsProcessor::~CommandsProcessor()
{
	stop_flag_ = false;
	cout_thr.join();
	file_thr1.join();
	file_thr2.join();
}

/**
*	Add command int the block
*/
void CommandsProcessor::PushPool(const std::string& s)
{
	if (pool_.size() == 0)
	{
		firstCmdTime_ = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	pool_.push_back(s);
}

/**
*	Output string
*/
void CommandsProcessor::PrintString(const std::string& s)
{
	{
		lock_guard<std::mutex> guard(ThreadCoutMutex);
		stringsQueueCout.push(s);
	}
	{
		lock_guard<std::mutex> guard(ThreadFileMutex);
		stringsQueueFile.push(pair(s, firstCmdTime_));
	}
}

/**
*	Output block of commands
*/
void CommandsProcessor::PrintPool()
{
	if (pool_.size())
	{
		string s{ std::to_string(reinterpret_cast<long long>(this)) + " bulk: "};

		for(size_t i=0; i < pool_.size(); ++i)
		{
			s += pool_[i];

			if (i != pool_.size() - 1)
			{
				s += ", ";
			}
		}
		s += "\n";
		PrintString(s);
		pool_.clear();
	}
}

bool CommandsProcessor::ProcessCommand(const std::string& cmd) 
{
	bool res;

	ICommandHandlerPtr ptr = handler_->ProcessCommand(this, cmd, res);

	if (ptr)
	{
		handler_ = std::move(ptr);
	}

	return !res;
}
