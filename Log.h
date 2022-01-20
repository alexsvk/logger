#pragma once
#include <cstdio>
#include <string>
#include <ctime>
#include <thread>
#include <sstream>
#include <fstream>
#include <mutex>
#include <memory>
enum priority {INFO=4, DEBUG=3, WARNING=2, ERROR=1};

std::string priority_as_string(priority pri);

class Log;
Log getLogger(const std::string& file_path = "", const std::string& ctx = "");
void ter_write(const std::string&);
void file_write(const std::string&, const std::string&);

class Log
{
public:

	~Log()
	{
		flush();
	}

	std::string lock_file_path() const
	{
		return file_name + ".lock";
	}

	Log& operator()(priority pri)
	{
		pri = pri;
		return *this;
	}

	template <typename Arg>
	Log& operator<<(const Arg& param)
	{
		std::ostringstream ss;
		ss << param;
		buffer += ss.str();
		return *this;
	}

	void flush();
private:
	
	Log(const std::string& file_path, // can be empty
		const std::string& ctx) : file_name(file_path), context(ctx)
	{
	}

	friend Log getLogger(const std::string& file_path, const std::string&);

	std::string file_name;
	std::string context;
	priority pri = DEBUG;
	std::string buffer;
};


