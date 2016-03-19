#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <vector>
#include <memory>
#include "Experiment.h"

namespace task
{
	/**
		Class encapsulating the list of all experiments defined in the config
	*/
	class experiments
	{
	private:
		// TODO add master as friend class
		std::vector<std::shared_ptr<experiment>> experiments_list;
	public:
		/**
			Constructor receives a file a reads the experiments
		*/
		explicit experiments(std::string file);
		/**
			Given id, returns a pointer to the experiment - as a function or bracket operator
		*/
		std::shared_ptr<experiment> const& get_experiment(unsigned int id) const;
		std::shared_ptr<experiment> const& operator[](unsigned int id) const;

		unsigned int experiment_count() const { return experiments_list.size(); }
		std::shared_ptr<experiment> const& at(unsigned int index) const;
	};
}

#endif