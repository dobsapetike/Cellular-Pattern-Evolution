#ifndef OBJFUNC_H
#define OBJFUNC_H

#include "../../Lattice/Headers/Lattice.h"

namespace objective_functions
{
	/**
		An abstract class defining the interface of an objective
		function of the evolutionary algorithm
	*/
	class objective_func
	{
	private:
		std::string _name;
	protected:
		std::shared_ptr<lattice::lattice> lattice;
	public:
		objective_func(std::string name, std::shared_ptr<lattice::lattice> const& latt)
			: _name(name), lattice(latt) { }
		// evaluates the actual state of the lattice
		virtual double eval() = 0;
	};

	/**
		Objective function factory method, which creates 
		instance based on the function name
	*/
	unique_ptr<objective_func> parse_obj_func(std::string name, 
		std::shared_ptr<lattice::lattice> const& lattice);
}

#endif