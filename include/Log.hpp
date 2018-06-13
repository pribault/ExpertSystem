#ifndef LOG_HPP
# define LOG_HPP

# include <fstream>
// # include <time.h>

class	Log
{
	public:
		Log(const std::string &file);
		~Log(void);

		Log		&operator<<(std::ostream& (*pf)(std::ostream&));

		template	<typename T>
		Log		&operator<<(const T &s)
		{
			if (_start == true)
			{
				std::string	prefix;
				time_t		t = time(0);

				prefix.resize(20);
				prefix.resize(strftime(const_cast<char *>(prefix.c_str()), prefix.length(), "%G%m%d_%H%M%S", localtime(&t)));
				try
				{
					_file << "[" << prefix << "] " << s;
				}
				catch (const std::exception &e)
				{
					throw (e);
				}
				_start = false;
			}
			else
			{
				try
				{
					_file << s;
				}
				catch (const std::exception &e)
				{
					throw (e);
				}
			}
			return (*this);
		}
	private:
		std::ofstream	_file;
		bool			_start;
};

extern Log	log;

#endif
