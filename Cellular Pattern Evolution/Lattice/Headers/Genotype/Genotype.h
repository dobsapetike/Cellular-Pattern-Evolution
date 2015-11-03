#ifndef GENOTYPE_H
#define GENOTYPE_H 

#include "../Controllers/StateController.h"
#include "../Halting/StopCriterion.h"

namespace lattice
{
	namespace genotype
	{
		/**
			Genotype defines the inner representation of the individual,
			in our case the controller and the grammar for merging and splitting
		*/
		class genotype
		{
			// TODO split grammar
		private:
			unique_ptr<halting::stop_criterion> _stop_criterion;
			unique_ptr<controllers::state_controller> _controller;
		public:
			genotype(lattice_settings const& settings);
			void reset();
			// getters
			halting::stop_criterion& get_criterion() { return *_stop_criterion; }
			controllers::state_controller& get_controller() { return *_controller; };
		};
	}
}

#endif