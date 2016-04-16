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
		real_vector result_params;
		double result_fitness = DBL_MAX;
		optimizer::opt_solution pareto_sol;

		static bool running, save;
		static BOOL WINAPI handle_abort(DWORD c_event);
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
		void simulate(string folder = default_pic_path + "simulator", string file = "", bool observed = true);

		string experiment_name() const {
			return experiment_ptr->name + "(" + to_string(run_num) + ")";
		}
		static bool is_saved() { return save;  }
	};
}

#endif