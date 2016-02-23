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
	private:
		/**
			Needed for normalization
		*/
		unsigned int max_count;
	public:
		objective_func_cell_count(std::string name, double importance, std::shared_ptr<lattice::lattice> const& latt)
			: objective_func(name, importance, latt)
		{
			max_count = lattice->get_settings().width * lattice->get_settings().height;
		}
		/**
			Evaluation returns the number of cells
		*/ 
		virtual double eval() override { 
			return this->lattice.get()->get_phenotype()
				.expose_cells().size() / static_cast<double>(max_count); 
		};
	};
}

#endif