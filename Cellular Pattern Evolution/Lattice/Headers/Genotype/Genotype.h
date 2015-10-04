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
		// TODO grammar, methods
		public:
			unique_ptr<controllers::state_controller> _controller;	// to private
			genotype(lattice_settings const& settings);
		};
	}
}

#endif