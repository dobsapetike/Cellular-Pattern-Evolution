#ifndef LATTICESTATISTICS_H
#define LATTICESTATISTICS_H

#include <string>
#include <vector>

namespace lattice
{
	struct obj_func_stat
	{
		double best_fitn = DBL_MAX, avg_fitn = 0, std_dev_fitn = 0;
	};

	struct statistics_log
	{
		unsigned int generation, eval_count;
		std::vector<obj_func_stat> objective_stats;
	};

	struct observed_run_stat
	{
		double energy, energy_dev, color_distance;
		unsigned int cell_count;
		unsigned int merge_count, split_count;

		void reset() {
			energy = energy_dev = 0.0;
			merge_count = split_count = 0;
		}
	};

	class lattice_statistics
	{
	public:
		unsigned int sim_eval_count;
		unsigned int eval_count = 0, gen_count = 0;

		observed_run_stat observed_run_stat;

		void reset() {
			sim_eval_count = 0;
			observed_run_stat.reset();
		}
	};
}

#endif