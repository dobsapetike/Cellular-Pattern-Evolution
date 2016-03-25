#ifndef TASK_H
#define TASK_H

#include "Experiment.h"
#include "../Main/Headers/Plotter.h"
#include "../Main/Headers/Painter.h"
#include "../../Lattice/Headers/Lattice.h"
#include "../../Optimizer/Headers/Optimizer.h"

#include <Windows.h>

namespace task
{
	/**
		Class performing an experiment
	*/
	class task
	{
	private:
		std::unique_ptr<optimizer::optimizer> optimizer;
		std::shared_ptr<objective_functions::ca_multiobj_func> obj_func;
		std::shared_ptr<lattice::lattice> lattice;

		std::unique_ptr<plotter> plotter;
		std::unique_ptr<painter> painter;
		std::unique_ptr<experiment> experiment_ptr;

		unsigned int run_num = 0;
		real_vector result;
		double result_fitness = DBL_MAX;

		static bool running;
		static BOOL WINAPI handle_abort(DWORD c_event);

		const unsigned int observable_gen_count = 5;
	public:
		explicit task(experiment const& exp, unsigned int num);
		/**
			Executes the experiment
		*/
		void execute();
		/**
			Finalizes the experiment - should be called after the execution
		*/
		void finalize();
		void generate_video();
		void simulate();

		string experiment_name() const {
			return experiment_ptr->name + "(" + to_string(run_num) + ")";
		}
	};
}

#endif