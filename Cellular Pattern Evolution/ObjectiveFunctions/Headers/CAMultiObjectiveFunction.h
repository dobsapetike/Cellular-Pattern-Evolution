#ifndef MULTIOBJFUNC_H
#define MULTIOBJFUNC_H

#include <vector>
#include <shark/ObjectiveFunctions/AbstractObjectiveFunction.h>
#include <shark/ObjectiveFunctions/BoxConstraintHandler.h>

#include "ObjectiveFunction.h"
#include "ObjectiveSettings.h"

using namespace std;
using namespace shark;

namespace objective_functions
{
	/**
		Class encapsulating all the objective functions of the optimization
		and implementing the interface for to be used by the optimizer
	*/
	class ca_multiobj_func final : public MultiObjectiveFunction
	{
	private:
		// constraints for the search space
		BoxConstraintHandler<SearchPointType> _constraintHandler;
		// list of all the objective functions
		vector<unique_ptr<objective_func>> _objectives;
		// pointer to the lattice
		shared_ptr<lattice::lattice> _lattice;
	public:
		ca_multiobj_func(
			objective_settings const& settings,			// config settings
			unsigned int dimensions,					// number of variables
			shared_ptr<lattice::lattice> const& lattice	// pointer to the lattice
		);
		// interface implementation
		string name() const { return "CA MO Fitness Function"; }
		size_t numberOfObjectives() const { return _objectives.size(); }
		size_t numberOfVariables() const { return _constraintHandler.dimensions(); }
		bool hasScalableObjectives() const { return false; }
		bool hasScalableDimensionality() const { return false; }
		// evaluates the objective functions
		ResultType eval(const SearchPointType& x) const;
	};
}

#endif