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
			unsigned int _window_size = 10, _step_limit = 200;
			double _threshold = 10e-15, _history_sum = 0.0;
			vector<double> _energy_history;

			double compute_energy(phenotypes::lattice_cell& cell) const;
		public:
			energy_halting(lattice_settings const& settings);
			virtual void reset() override { 
				_history_sum = 0;
				_energy_history.clear();
			};
			// decides whether the simulation should stop
			virtual bool should_stop(lattice& statistics) override;
		};
	}
}

#endif