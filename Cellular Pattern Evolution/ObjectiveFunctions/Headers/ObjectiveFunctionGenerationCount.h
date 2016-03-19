#ifndef OBJFUNC_GENCOUNT_H
#define OBJFUNC_GENCOUNT_H

#include "ObjectiveFunction.h"

namespace objective_functions
{
	/**
		Objective function which tries to minimize the number of generations of the simulation
	*/
	class objective_func_gen_count : public objective_func
	{
	private:
		/**
			Needed for normalization
		*/
		unsigned int max_count;
	public:
		objective_func_gen_count(std::string name, double importance, std::shared_ptr<lattice::lattice> const& latt)
			: objective_func(name, importance, latt)
		{
			max_count = lattice->get_genotype().get_criterion().get_step_limit();
		}
		/**
			Evaluation returns the number of generations
		*/
		virtual double eval() override {
			return this->lattice.get()->get_statistics().
				sim_eval_count / static_cast<double>(max_count);
		};
	};
}

#endif