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
		std::unique_ptr<optimizer::optimizer> _optimizer;
		std::shared_ptr<objective_functions::ca_multiobj_func> _obj_func;
		std::shared_ptr<lattice::lattice> _lattice;

		std::unique_ptr<plotter> _plotter;
		std::unique_ptr<painter> _painter;
		std::unique_ptr<experiment> _experiment;

		real_vector _result;
		double _result_fitness = DBL_MAX;

		static bool _running;
		static BOOL WINAPI _handle_abort(DWORD c_event);
	public:
		explicit task(experiment const& exp);
		/**
			Executes the experiment
		*/
		void execute();
		/**
			Finalizes the experiment - should be called after the execution
		*/
		void finalize();
	};
}

#endif