#include "Log.hpp"

Log	log("log");

Log::Log(const std::string &file)
{
	try
	{
		_file.open(file.c_str(), std::ofstream::out | std::ofstream::app);
		_file << "___ Log start ___" << std::endl;
	}
	catch (const std::exception &e)
	{
		throw (e);
	}
	_start = true;
}

Log::~Log(void)
{
	_file.close();
}

Log		&Log::operator<<(std::ostream& (*pf)(std::ostream&))
{
	if (!pf)
		throw (std::exception());
	try
	{
		pf(_file);
	}
	catch (const std::exception &e)
	{
		throw (e);
	}
	_start = true;
	return (*this);
}
