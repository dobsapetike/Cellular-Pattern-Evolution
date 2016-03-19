#include "Headers/ObjectiveFunction.h"
#include "Headers/ObjectiveFunctionCellCount.h"
#include "Headers/ObjectiveFunctionColorDistance.h"
#include "Headers/ObjectiveFunctionGenerationCount.h"
#include <algorithm>

namespace objective_functions
{
	/**
		Objective function factory method, which creates
		instance based on the function name
	*/
	unique_ptr<objective_func> parse_obj_func(std::string name, double importance,
		std::shared_ptr<lattice::lattice> const& lattice)
	{
		string oName = name;
		transform(name.begin(), name.end(), name.begin(), ::tolower);

		if (name == "cellcount")
			return unique_ptr<objective_func_cell_count>(new objective_func_cell_count(oName, importance, lattice));
		if (name == "colordist")
			return unique_ptr<objective_func_color_dist>(new objective_func_color_dist(oName, importance, lattice));
		if (name == "gencount")
			return unique_ptr<objective_func_gen_count>(new objective_func_gen_count(oName, importance, lattice));

		throw invalid_argument("Unknown objective function: " + oName);
	}
}