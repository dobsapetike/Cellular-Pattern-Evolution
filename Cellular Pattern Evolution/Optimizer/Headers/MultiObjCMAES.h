#ifndef MOCMA_OPT_H
#define MOCMA_OPT_H

#include <shark/Algorithms/DirectSearch/MOCMA.h>
#include "Optimizer.h"

using namespace shark;

namespace optimizer
{
	/**
		Multi-objective CMA evolutionary strategy
		Wrapper around the shark library implementation
	*/
	class mo_cmaes_optimizer : public optimizer
	{
	private:
		MOCMA _mocma;
		unsigned int _max_step_count, _current_step_count = 0;
	public:
		mo_cmaes_optimizer(
			std::shared_ptr<objective_functions::ca_multiobj_func> const& objfunc,
			optimizer_settings const& settings);

		unsigned int step_count() const { return _current_step_count; }

		std::string get_name() const override { return "MOCMA"; };
		void init() override;
		void step() override;
		bool should_stop() override;
		// Gets the solution of the MO-CMAES optimizer 
		// and converts it to the solution type used by this framework
		virtual opt_solution get_solution() override;
	};
}

#endif