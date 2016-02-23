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
		BoxConstraintHandler<SearchPointType> constraintHandler;
		// list of all the objective functions
		vector<unique_ptr<objective_func>> objectives;
		// pointer to the lattice
		shared_ptr<lattice::lattice> lattice;
	public:
		ca_multiobj_func(
			objective_settings const& settings,			// config settings
			unsigned int dimensions,					// number of variables
			shared_ptr<lattice::lattice> const& lattice	// pointer to the lattice
		);
		// interface implementation
		string name() const override { return "CA MO Fitness Function"; }
		size_t numberOfObjectives() const override { return objectives.size(); }
		size_t numberOfVariables() const override { return constraintHandler.dimensions(); }
		bool hasScalableObjectives() const override { return false; }
		bool hasScalableDimensionality() const override { return false; }
		// evaluates the objective functions
		ResultType eval(const SearchPointType& x) const override;
	};
}

/**
	Conversion between the two kinds of vectors used in this class
	needed for compatibility reasons with the shark library
*/
RealVector vector_convert(real_vector const& vec);
real_vector vector_convert(RealVector const& vec);

#endif