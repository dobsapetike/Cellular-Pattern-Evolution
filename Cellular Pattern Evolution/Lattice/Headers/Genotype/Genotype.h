#ifndef GENOTYPE_H
#define GENOTYPE_H 

#include "../Controllers/StateController.h"

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
			unique_ptr<controllers::state_controller> _controller;
			// TODO grammar, methods
		public:
			genotype(lattice_settings const& settings);
			// getters
			const controllers::state_controller& get_controller() const { return *_controller; };
		};
	}
}

#endif