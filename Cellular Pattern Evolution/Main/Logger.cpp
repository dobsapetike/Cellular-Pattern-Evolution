#include "Headers/Logger.h"
#include <boost/filesystem.hpp>

logger::logger()
{
	boost::filesystem::create_directory(path);
	_outfile_info.open(path + "info.log");
	_outfile_error.open(path + "error.log");
}

/**
	Dump info string to the outputs
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
	dump("[INFO] " + info, _outfile_info);
}

/**
	Log error message
*/
void logger::log_error(string const& error)
{
	dump("[ERROR] " + error, _outfile_error);
}

/**
Log error message
*/
void logger::log_evol_stat(string const& stat)
{
	dump("[EVOL]" + stat);
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
void logger::log_experiment_end()
{
	_outfile_evol.close();
}