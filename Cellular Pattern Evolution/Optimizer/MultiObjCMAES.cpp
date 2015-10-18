#include "Headers/MultiObjCMAES.h"
#include <iostream>

namespace optimizer
{
	mo_cmaes_optimizer::mo_cmaes_optimizer(
		std::shared_ptr<objective_functions::ca_multiobj_func> const& objfunc,
		optimizer_settings const& settings) : optimizer(objfunc)
	{
		_mocma.initialSigma() = atof(settings.params->FirstChildElement("InitStepSize")->GetText());
		_mocma.mu() = atoi(settings.params->FirstChildElement("ParentSelectionCount")->GetText());
		_max_step_count = atoi(settings.params->FirstChildElement("EvolutionStepCount")->GetText());
	};

	/**
		Gets the solution of the MO-CMAES optimizer
		and converts it to the solution type used by this framework
	*/
	opt_solution mo_cmaes_optimizer::get_solution()
	{
		opt_solution sol;
		for (unsigned int i = 0; i < _mocma.solution().size(); ++i)
		{
			real_vector points, values;
			auto cmaSol = _mocma.solution()[i];
			points.insert(points.end(), cmaSol.point.begin(), cmaSol.point.end());
			values.insert(values.end(), cmaSol.value.begin(), cmaSol.value.end());
			sol.push_back(opt_result(points, values));
		}
		return sol;
	};

	void mo_cmaes_optimizer::init()
	{
		_mocma.init(*objective_func);
		_current_step_count = 0;
		cout << "Started evolution " << endl;
	}

	void mo_cmaes_optimizer::step()
	{
		_mocma.step(*objective_func);
		++_current_step_count;
		cout << "Current step count: " << _current_step_count << endl;
	}

	/**
		Stop after predetermined number of steps
	*/
	bool mo_cmaes_optimizer::should_stop()
	{
		return _current_step_count >= _max_step_count;
	}
}