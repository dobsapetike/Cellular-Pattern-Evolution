#ifndef TASK_H
#define TASK_H

#include "Experiment.h"
#include "../Main/Headers/Plotter.h"
#include "../../Lattice/Headers/Lattice.h"
#include "../../Optimizer/Headers/Optimizer.h"

namespace task
{
	class task
	{
	private:
		std::unique_ptr<optimizer::optimizer> _optimizer;
		std::shared_ptr<objective_functions::ca_multiobj_func> _obj_func;
		std::shared_ptr<lattice::lattice> _lattice;

		std::unique_ptr<plotter> _plotter;
		std::unique_ptr<experiment> _experiment;
	public:
		explicit task(experiment const& exp);
		void execute();
	};
}

#endif