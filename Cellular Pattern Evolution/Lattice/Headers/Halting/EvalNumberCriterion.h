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
		private:
			unsigned int _eval_limit;
		public:
			eval_num_halting(lattice_settings const& settings);
			// decides whether the simulation should stop
			virtual bool should_stop(lattice_statistics const& statistics) const override;
		};
	}
}

#endif