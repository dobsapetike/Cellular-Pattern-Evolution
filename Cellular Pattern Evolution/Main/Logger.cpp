#include "Headers/Logger.h"
#include <ctime>
#include <boost/filesystem.hpp>

logger::logger()
{
	boost::filesystem::create_directory(path);
	outfile_info.open(path + "info.log");
	outfile_error.open(path + "error.log");
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
	dump(s, outfile_evol);
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
	dump("[INFO][" + get_time() + "] " + info, outfile_info);
}

/**
	Log warning information
*/
void logger::log_warning(string const& info)
{
	dump("[WARNING][" + get_time() + "] " + info, outfile_info);
}

/**
	Log error message
*/
void logger::log_error(string const& error)
{
	dump("[ERROR][" + get_time() + "] " + error, outfile_error);
}

/**
	Log evolution info
*/
void logger::log_evol_stat(lattice::statistics_log const& stat)
{
	outfile_evol << get_time() << "," << stat.generation << "," << stat.eval_count << ",";
	for (unsigned int i = 0; i < stat.objective_stats.size(); ++i) {
		auto& os = stat.objective_stats[i];
		outfile_evol << os.best_fitn << "," << os.avg_fitn << "," << os.std_dev_fitn;
		if (i < stat.objective_stats.size() - 1) outfile_evol << ",";
	}
	outfile_evol << endl;
	outfile_evol.flush();
}

/**
	Signal the start of a new experiment. New log file is created.
*/
void logger::log_experiment_start(string const& exp_name, vector<string>& objectives)
{
	outfile_evol.open(path + exp_name + ".log");
	// set the log file header
	outfile_evol << "Timestamp,Gen_num,Eval_num,";
	for (unsigned int i = 0; i < objectives.size(); ++i) {
		string o = objectives[i];
		outfile_evol << "Best_fitn_" << o << ",Avg_fitn_" << o << ",Std_dev_fitn_" << o;
		if (i < objectives.size() - 1) outfile_evol << ",";
	}
	outfile_evol << endl;
	outfile_evol.flush();

	dump(string(28 + exp_name.size(), '*'), outfile_info);
	dump("*** Experiment started: " + exp_name + " ***", outfile_info);
	dump(string(28 + exp_name.size(), '*'), outfile_info);
	dump("", outfile_info);
}

/**
	Signal the end the current experiment. Log file closes.
*/
void logger::log_experiment_end(string const& exp_name)
{
	dump(string(28 + exp_name.size(), '*'), outfile_info);
	dump("Experiment ended!", outfile_info);
	dump(string(28 + exp_name.size(), '*'), outfile_info);

	outfile_evol.close();
}

void logger::log_observed_run_start(unsigned int gen)
{
	observed_run_stream.open(path + "observed_gen" + to_string(gen) + ".log");
	observed_run_stream << "Gen,Energy,Color_dist,Cell_count,Merge_count,Split_count" << endl;
	observed_run_stream.flush();
}

void logger::log_observed_run_gen(unsigned int gen, lattice::observed_run_stat const& stat)
{
	observed_run_stream << gen << "," << stat.energy << "," << stat.color_distance << ","
		<< stat.cell_count << "," << stat.merge_count << "," << stat.split_count << endl;
	observed_run_stream.flush();
}

void logger::log_observed_run_end()
{
	observed_run_stream.close();
}