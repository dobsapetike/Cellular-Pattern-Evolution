#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <vector>
#include <shark/Core/ResultSets.h>

#include "OptimizerSettings.h"
#include "../../ObjectiveFunctions/Headers/CAMultiObjectiveFunction.h"
#include "../../Main/Headers/CommonTypes.h"

using namespace common_types;

namespace optimizer
{
	// type for the evolution result: the solution is the pareto front (vector of possible results)
	// which consists of a tuple of vectors: parameters (searchpoints) and the objective function values
	typedef std::pair<real_vector, real_vector> opt_result;
	typedef vector<opt_result> opt_solution;

	/**
		Interface for implementing the logic of 
		the state controller parameter optimization
	*/
	class optimizer
	{
	protected:
		std::shared_ptr<objective_functions::ca_multiobj_func> objective_func;
	public:
		optimizer(std::shared_ptr<objective_functions::ca_multiobj_func> const& objfunc) 
			: objective_func(objfunc) { }
		// name of the algorithm
		virtual std::string get_name() const = 0;

		// Initialization of the algorithm in order to set up instances, 
		// reset states, etc...
		// Always called before the start of the optimization
		virtual void init() = 0;
		// Makes one step of the optimization, afterr each step the solutin should be updated
		virtual void step() = 0;
		// Whether the optimization should be stopped after the actual step
		virtual bool should_stop() = 0;

		// Returns the current solution
		virtual opt_solution get_solution() = 0;
	};

	//for (std::size_t i = 0; i < 1; i++) { // mocma.solution().size()
	//	for (std::size_t j = 0; j < tf.numberOfObjectives(); j++) {
	//		cout << mocma.solution()[0].point[0] << " || " << mocma.solution()[0].point[1] << endl;
	//		std::cout << mocma.solution()[i].value[j] << endl;
	//	}
	//}

	/**
		Optimizer factory method, constructing an optimizer instance based on settings
	*/
	std::unique_ptr<optimizer> create_optimizer(
		optimizer_settings const& settings,
		std::shared_ptr<objective_functions::ca_multiobj_func> const& objfunc
	);
}

#endif