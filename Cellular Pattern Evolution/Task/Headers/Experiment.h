#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <string>

namespace task
{
	/**
		Structure containig experiment information
	*/
	struct experiment
	{
		std::string name;
		unsigned int id;
		unsigned int run_count;
		// detailed informations are in their respective files
		std::string controller_file;
		std::string lattice_file;
		std::string objective_file;
		std::string optimizer_file;
		std::string stopcriterion_file;
	};
}

#endif