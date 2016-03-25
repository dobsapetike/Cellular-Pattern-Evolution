#include "Headers/MultiObjCMAES.h"
#include "../Main/Headers/Logger.h"
#include <shark/Rng/GlobalRng.h>

namespace optimizer
{
	mo_cmaes_optimizer::mo_cmaes_optimizer(
		std::shared_ptr<objective_functions::ca_multiobj_func> const& objfunc,
		optimizer_settings const& settings) : optimizer(objfunc)
	{
		mocma.initialSigma() = atof(settings.params->FirstChildElement("InitStepSize")->GetText());
		mocma.mu() = atoi(settings.params->FirstChildElement("ParentSelectionCount")->GetText());
		max_step_count = max(atoi(settings.params->FirstChildElement("EvolutionStepCount")->GetText()), 5);

		unsigned int time_ui = static_cast<unsigned int>(time(nullptr));
		Rng::seed(time_ui);
	};

	/**
		Gets the solution of the MO-CMAES optimizer
		and converts it to the solution type used by this framework
	*/
	opt_solution mo_cmaes_optimizer::get_solution()
	{
		opt_solution sol;
		for (unsigned int i = 0; i < mocma.solution().size(); ++i)
		{
			real_vector points, values;
			auto& cmaSol = mocma.solution()[i];
			points.insert(points.end(), cmaSol.point.begin(), cmaSol.point.end());
			values.insert(values.end(), cmaSol.value.begin(), cmaSol.value.end());
			sol.push_back(opt_result(points, values));
		}
		return sol;
	};

	void mo_cmaes_optimizer::init()
	{
		logger::get_logger().log_info("Initializing optimizer!");

		mocma.init(*objective_func);
		current_step_count = 0;
		
		logger::get_logger().log_info("Initialization complete! Starting evolution!");
	}

	void mo_cmaes_optimizer::step()
	{
		logger::get_logger().log_info("Generation: " + to_string(current_step_count + 1));

		mocma.step(*objective_func);
		++current_step_count;
	}

	/**
		Stop after predetermined number of steps
	*/
	bool mo_cmaes_optimizer::should_stop()
	{
		return current_step_count >= max_step_count;
	}
}