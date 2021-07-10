#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <mutex>

/**
*	Write in the std::cout
*/
class CoutWriter
{
public:

	CoutWriter() = default;
	~CoutWriter() = default;

	void Print(const std::string& s);

private:

	static std::mutex cout_mutex_;
};

/**
*	Write in the file
*/
class FileWriter
{
public:

	FileWriter() = default;
	~FileWriter() = default;

	void Print(long long time, int postfix, const std::string& s);

private:

	std::ofstream fileStream_;
	long long time_ = 0;
};