#include "Headers/Task.h"
#include "../Main/Headers/Logger.h"

namespace task
{
	task::task(experiment const& exp)
	{
		// initialize fields
		try
		{
			_lattice = std::make_shared<lattice::lattice>(lattice::load_settings(
				exp.lattice_file, exp.controller_file, exp.stopcriterion_file));

			_obj_func = std::make_shared<objective_functions::ca_multiobj_func>(
				*objective_functions::load_settings(exp.objective_file),
				_lattice->get_genotype().get_controller().get_param_count(),
				_lattice
			);
			_optimizer = std::unique_ptr<optimizer::optimizer>(
				optimizer::create_optimizer(
					*optimizer::load_settings(exp.optimizer_file),
					_obj_func
				)
			);

			_experiment = make_unique<experiment>(exp);
		}
		catch (std::invalid_argument& ia)
		{
			logger::get_logger().log_error(ia.what());
		}
	}

	void task::execute()
	{
		logger::get_logger().log_experiment_start(_experiment->name);

		_optimizer->init();
		while (!_optimizer->should_stop())
		{
			_optimizer->step();

			auto sol = _optimizer->get_solution();

			double min = DBL_MAX;
			for (unsigned int i = 0; i < sol.size(); ++i)
			{
				min = std::min(min, sol[i].second[0]);
			}

			logger::get_logger().log_evol_stat("Fitness: " + to_string(sol[0].second[0]));
		}

		logger::get_logger().log_experiment_end();
	}
}