#include "writer.h"

std::mutex CoutWriter::cout_mutex_;

void CoutWriter::Print(const std::string& s)
{
	std::lock_guard<std::mutex> guard(cout_mutex_);
	std::cout << s;
}

void FileWriter::Print(long long time, int postfix, const std::string& s)
{
	fileStream_.open("bulk_" + std::to_string(time) + "_" + std::to_string(postfix) + ".log", std::ios::app);
	fileStream_ << s;
	fileStream_.close();
}
