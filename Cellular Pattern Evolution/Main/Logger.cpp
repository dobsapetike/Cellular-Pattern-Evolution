#include "Headers/Logger.h"
#include <ctime>
#include <boost/filesystem.hpp>

logger::logger()
{
	boost::filesystem::create_directory(path);
	_outfile_info.open(path + "info.log");
	_outfile_error.open(path + "error.log");
}

/**
	Returns a string containing the local time
*/
string get_time()
{
	string res("");
	time_t now = time(nullptr);
	tm *ltm = localtime(&now);

	auto format_num = [](string s) 
		{ return s.length() == 2 ? s : "0" + s; };

	res += format_num(to_string(ltm->tm_hour));
	res += ":" + format_num(to_string(ltm->tm_min));
	res += ":" + format_num(to_string(ltm->tm_sec));
	return res;
}

/**
	Dump the info string to the outputs
*/
void logger::dump(string const& s)
{
	dump(s, _outfile_evol);
}

void logger::dump(string const& s, ofstream& stream) const
{
	cout << s << endl;
	if (stream.is_open())
	{
		stream << s << endl;
		stream.flush();
	}
}

/**
	Log standard information
*/
void logger::log_info(string const& info)
{
	dump("[INFO][" + get_time() + "] " + info, _outfile_info);
}

/**
	Log warning information
*/
void logger::log_warning(string const& info)
{
	dump("[WARNING][" + get_time() + "] " + info, _outfile_info);
}

/**
	Log error message
*/
void logger::log_error(string const& error)
{
	dump("[ERROR][" + get_time() + "] " + error, _outfile_error);
}

/**
	Log error message
*/
void logger::log_evol_stat(string const& stat)
{
	dump("[EVOL][" + get_time() + "] " + stat);
}

/**
	Signal the start of a new experiment. New log file is created.
*/
void logger::log_experiment_start(string exp_name)
{
	_outfile_evol.open(path + exp_name + ".log");

	dump(string(28 + exp_name.size(), '*'));
	dump("*** Experiment started: " + exp_name + " ***");
	dump(string(28 + exp_name.size(), '*'));
	dump("");
}

/**
	Signal the end the current experiment. Log file closes.
*/
void logger::log_experiment_end(string exp_name)
{
	dump(string(28 + exp_name.size(), '*'));
	dump("Experiment ended!");
	dump(string(28 + exp_name.size(), '*'));

	_outfile_evol.close();
}