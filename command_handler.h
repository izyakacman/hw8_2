#pragma once

#include <string>
#include <memory>

class CommandsProcessor;
constexpr auto EndOfFileString = "eof";

class ICommandHandler;
using ICommandHandlerPtr = std::unique_ptr<ICommandHandler>;

/**
*	Command handler interface
*/
class ICommandHandler {
public:

	ICommandHandler() = delete;
	explicit ICommandHandler(size_t count) : count_{ count } {}
	virtual ~ICommandHandler() = default;

    virtual ICommandHandlerPtr ProcessCommand(CommandsProcessor*, const std::string&, bool& exit) = 0;

protected:

	const size_t count_;
};

/**
*	Process command int the static mode
*/
class StaticCommandHandler : public ICommandHandler
{
public:

	StaticCommandHandler() = delete;
	StaticCommandHandler(size_t count) : ICommandHandler(count) {}
	~StaticCommandHandler() = default;

	ICommandHandlerPtr ProcessCommand(CommandsProcessor* cmd, const std::string& s, bool& exit) override;
};

/**
*	Process command int the dynamic mode
*/
class DynamicCommandHandler : public ICommandHandler
{
public:

	DynamicCommandHandler() = delete;
	DynamicCommandHandler(size_t count) : ICommandHandler(count) {}
	~DynamicCommandHandler() = default;

	ICommandHandlerPtr ProcessCommand(CommandsProcessor* cmd, const std::string& s, bool& exit) override;

private:

	size_t openBraceCount_ = 0;
};