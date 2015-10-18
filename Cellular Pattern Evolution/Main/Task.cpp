#include "Headers/Task.h"

#include <iostream>

namespace task
{
	task::task(std::string configPath)
	{
		// initialize fields
		_lattice = std::shared_ptr<lattice::lattice>(
			new lattice::lattice(lattice::load_settings(configPath))
		);
		_obj_func = std::shared_ptr<objective_functions::ca_multiobj_func>(
			new objective_functions::ca_multiobj_func(
				*objective_functions::load_settings(configPath),
				_lattice.get()->get_genotype().get_controller().get_param_count(),
				_lattice
			)
		);
		_optimizer = std::unique_ptr<optimizer::optimizer>(
			optimizer::create_optimizer(
				*optimizer::load_settings(configPath),
				_obj_func
			)
		);
	}

	void task::execute()
	{
		auto opt = _optimizer.get();
		opt->init();
		while (!opt->should_stop())
		{
			opt->step();

			auto sol = opt->get_solution();
			cout << "Number of solutions: " << sol.size() << endl;
			std::cout << "Fitness: " << sol[0].second[0] << endl;
		}
	}
}