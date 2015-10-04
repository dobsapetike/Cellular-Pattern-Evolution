#ifndef STATE_H
#define STATE_H

#include "../../Main/Headers/CommonTypes.h"

namespace lattice
{
	/**
		Structure representing the state of a single CA cell.
		The state consists of two real vectors - called chemicals: 
			- internal used in the computation of the next state of the current cell
			- external used in the computation of the next state of the neighbouring cells
	*/
	struct state
	{
		common_types::real_vector internal_chemicals;
		common_types::real_vector external_chemicals;
	};

	/**
		Structure for describing state and state related properties.
		Used for storing config settings.
	*/
	struct state_settings
	{
		// chemical vector length
		unsigned int internal_chemical_count;
		unsigned int external_chemical_count;

		// initial chemical values
		double init_val;

		// TODO gaussian blur settings
	};

	/**
		Initializes the states based on input settings
	*/
	state init_state(state_settings const& settings);
}

#endif