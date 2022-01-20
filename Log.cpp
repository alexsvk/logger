#include "Log.h"

#include <mutex>
#include <iostream>
#include <fstream>

namespace
{
	std::mutex ter_mtx;
	std::mutex file_mtx;
	std::mutex mtx; // used in Log::flush
}

std::string priority_as_string(priority pri)
{
	switch (pri)
	{
	case priority::INFO:
		return "INFO";
	case priority::DEBUG:
		return "DEBUG";
	case priority::WARNING:
		return "WARNING";
	case priority::ERROR:
		return "ERROR";
	default:
		break;
	}
}

Log getLogger(const std::string& file_path, const std::string& ctx)
{
	return Log(file_path, ctx);
}

void ter_write(const std::string& param)
{
	std::lock_guard<std::mutex> guard(ter_mtx);
	std::cout << param;
}

void file_write(const std::string& path, const std::string& input)
{
	std::lock_guard<std::mutex> guard(file_mtx);
	std::ofstream fs(path, std::ios::app);
	if (fs.is_open())
	{
		fs << input;
	}
}

void Log::flush()
{
	if (buffer.size() == 0)
	{
		return;
	}

	buffer += '\n';
	mtx.lock();
	time_t time_ = time(NULL); // since 1.1.1970
	tm* tmp = gmtime(&time_);
	char buf[64] = "\0";
	asctime_s(buf, tmp);
	mtx.unlock();
	std::ostringstream ss;
	ss << buf << ';' << priority_as_string(pri) << ';' << context << '(' << std::this_thread::get_id() << ')' << ':' << buffer;
	if (file_name.empty())
	{
		// terminal
		ter_write(ss.str());
	}
	else
	{
		// file
		file_write(file_name, ss.str());
	}

	buffer = "";
}