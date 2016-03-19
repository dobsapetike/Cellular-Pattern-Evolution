#ifndef EVAL_NUM_CRITERION_H
#define EVAL_NUM_CRITERION_H

#include "StopCriterion.h"

namespace lattice
{
	namespace halting
	{	
		/*
			Simple haling criterion: stop after a predefined number of steps
		*/
		class eval_num_halting : public stop_criterion
		{
		public:
			eval_num_halting(lattice_settings const& settings);
			virtual void reset() override { };
			// decides whether the simulation should stop
			virtual bool should_stop(lattice& lattice) override;
		};
	}
}

#endif