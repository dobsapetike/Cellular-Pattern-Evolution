#include "Headers/CAMultiObjectiveFunction.h"

#include <iostream>

namespace objective_functions
{
	ca_multiobj_func::ca_multiobj_func(
		objective_settings const& settings,			// config settings
		unsigned int dimensions,					// number of variables
		shared_ptr<lattice::lattice> const& lattice	// pointer to the lattice
		) : _lattice(lattice)
	{
		// iterate and remember objective functions
		TiXmlNode* nextChild = settings.objfunc_settings->FirstChild();
		if (nextChild == NULL)
			throw invalid_argument("No objective function specified!");

		while (nextChild != NULL)
		{
			TiXmlElement elem = *nextChild->ToElement();
			if (strcmpi(elem.Value(), "Function")) 
				throw invalid_argument("Objective function has to be specified as type \"Function\"!");

			string name = elem.Attribute("Name");
			_objectives.push_back(parse_obj_func(name, lattice));

			nextChild = nextChild->NextSibling();
		}
		
		// create and set the constraint handler
		_constraintHandler = BoxConstraintHandler<SearchPointType>(
			SearchPointType(dimensions, settings.min_value), 
			SearchPointType(dimensions, settings.max_value));
		announceConstraintHandler(&_constraintHandler);
	}

	/**
		Evaluates the objective functions and returns the vector of resulting values
	*/
	RealVector ca_multiobj_func::eval(const RealVector& x) const
	{
		common_types::real_vector params;
		params.insert(params.end(), x.begin(), x.end());

		// set new controller parameters and simulate run
		_lattice.get()->get_genotype().get_controller().set_params(params);
		_lattice.get()->simulate();

		ResultType result(numberOfObjectives());
		for (unsigned int i = 0; i < numberOfObjectives(); ++i)
		{
			result[i] = _objectives[i].get()->eval();
		}
		return result;
	}
}