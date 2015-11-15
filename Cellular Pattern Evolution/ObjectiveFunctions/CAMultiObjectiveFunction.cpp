#include "Headers/CAMultiObjectiveFunction.h"

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
		if (nextChild == nullptr)
			throw invalid_argument("No objective function specified!");

		while (nextChild != nullptr)
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
		real_vector params = vector_convert(x);
		// set new controller parameters and simulate run
		_lattice.get()->get_genotype().get_controller().set_params(params);
		_lattice.get()->simulate();

		ResultType result(numberOfObjectives());
		for (unsigned int i = 0; i < numberOfObjectives(); ++i)
		{
			result[i] = _objectives[i]->eval();
		}

		return result;
	}

}

/**
	Conversion between the two kinds of vectors used in this class
	needed for compatibility reasons with the shark library
*/
RealVector vector_convert(real_vector const& vec)
{
	RealVector v(vec.size());
	for (unsigned int i = 0; i < vec.size(); ++i)
		v[i] = vec[i];
	return v;
};

real_vector vector_convert(RealVector const& vec)
{
	real_vector v;
	v.insert(v.end(), vec.begin(), vec.end());
	return v;
}