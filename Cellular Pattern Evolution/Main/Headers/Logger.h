#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>

using namespace std;

/**
	Singleton logger, dumping info into console and the output file
*/
class logger
{
private:
	// private constructor and deleting methods which could copies
	logger();
	logger(logger const&)		  = delete;
	void operator=(logger const&) = delete;

	/**
		Dump info string to the outputs
	*/
	void dump(string const& s);
	void dump(string const& s, ofstream& stream) const;
		
	const string path = "logs/";	// path to the log files
	// 3 filestreams: for general info logging, errors and evolution statistics
	ofstream _outfile_evol, _outfile_info, _outfile_error;
public:
	/**
		Getter to the static instance
	*/
	static logger& get_logger()
	{
		static logger instance;
		return instance;
	}

	/**
		Log standard information
	*/
	void log_info(string const& info);
	/**
		Log warning information
	*/
	void logger::log_warning(string const& info);
	/**
		Log error message
	*/
	void log_error(string const& error);
	/**
	Log error message
	*/
	void log_evol_stat(string const& stat);
	/**
		Signal the start of a new experiment. New log file is created.
	*/
	void log_experiment_start(string exp_name);
	/**
		Signal the end the current experiment. Log file closes.
	*/
	void log_experiment_end(string exp_name);
};

#endif