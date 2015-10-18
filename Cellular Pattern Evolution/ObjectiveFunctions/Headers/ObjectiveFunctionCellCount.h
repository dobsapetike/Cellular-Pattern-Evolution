#ifndef OBJFUNC_CELLCOUNT_H
#define OBJFUNC_CELLCOUNT_H

#include "ObjectiveFunction.h"

namespace objective_functions
{
	/**
		Objective function which tries to minimize the number of cells in the CA
	*/
	class objective_func_cell_count : public objective_func
	{
	public:
		objective_func_cell_count(std::string name, std::shared_ptr<lattice::lattice> const& latt)
			: objective_func(name, latt) { }
		/**
			Evaluation returns the number of cells
		*/ 
		virtual double eval() override { 
			return this->lattice.get()->get_phenotype().expose_cells().size(); 
		};
	};
}

#endif