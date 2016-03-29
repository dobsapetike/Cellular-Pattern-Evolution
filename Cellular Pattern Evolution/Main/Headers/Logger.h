#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include "../Lattice/Headers/LatticeStatistics.h"

using namespace std;

/**
	Singleton logger, dumping info into console and the output file
*/
class logger
{
private:
	// private constructor and deleting methods which could create copies
	logger();
	logger(logger const&)		  = delete;
	void operator=(logger const&) = delete;

	/**
		Dump info string to the outputs
	*/
	void dump(string const& s);
	void dump(string const& s, ofstream& stream) const;
		
	const string path = "logs/";	// path to the log files
	string current_experiment;		// remember it
	// 3 persistent filestreams: for general info logging, errors and evolution statistics
	ofstream outfile_evol, outfile_info, outfile_error;

	// one for the observed runs
	ofstream observed_run_stream;
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
		Log evolution info
	*/
	void log_evol_stat(lattice::statistics_log const& stat);
	/**
		Signal the start of a new experiment. New log file is created.
	*/
	void log_experiment_start(string const& exp_name, vector<string>& objectives);
	/**
		Signal the end of the current experiment. Log file closes.
	*/
	void log_experiment_end(string const& exp_name);
	/**
		Signal the start of the current observed run. Log file closes.
	*/
	void log_observed_run_start(unsigned int gen);
	/**
		Log observed run info
	*/
	void log_observed_run_gen(unsigned int gen, lattice::observed_run_stat const& stat);
	/**
		Signal the end of the current observed run. Log file closes.
	*/
	void log_observed_run_end();
};

#endif