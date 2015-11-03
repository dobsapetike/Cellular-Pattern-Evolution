#include <string>
#include "Headers/Optimizer.h"
#include "Headers/MultiObjCMAES.h"

namespace optimizer
{
	/**
		Optimizer factory method, constructing an optimizer instance based on settings
	*/
	std::unique_ptr<optimizer> create_optimizer(
		optimizer_settings const& settings,
		std::shared_ptr<objective_functions::ca_multiobj_func> const& objfunc
		)
	{
		if (strcmpi(settings.algorithm.c_str(), "mocmaes") == 0)
			return std::make_unique<mo_cmaes_optimizer>(objfunc, settings);

		throw invalid_argument(
			"Unknown optimizer: " + settings.algorithm);
	}
}