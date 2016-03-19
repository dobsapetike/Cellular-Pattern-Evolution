#ifndef ENERGY_CRITERION_H
#define ENERGY_CRITERION_H

#include "StopCriterion.h"
#include "../Headers/Phenotypes/Cell/Cell.h"

namespace lattice
{
	namespace halting
	{
		/*
			Energy halting criterion based on Devert
		*/
		class energy_halting : public stop_criterion
		{
		private:
			unsigned int window_size = 10;
			double threshold = 10e-15, history_sum = 0.0;
			vector<double> energy_history;

			double compute_energy(phenotypes::lattice_cell& cell) const;
		public:
			energy_halting(lattice_settings const& settings);
			virtual void reset() override { 
				history_sum = 0;
				energy_history.clear();
			};
			// decides whether the simulation should stop
			virtual bool should_stop(lattice& statistics) override;
		};
	}
}

#endif