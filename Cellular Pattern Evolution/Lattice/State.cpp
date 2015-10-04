#include "Headers\State.h"
#include <iostream>

using namespace std;

namespace lattice
{
	/**
		Initializes the states based on input settings
	*/
	state init_state(state_settings const& settings)
	{
		state state;

		state.external_chemicals = common_types::real_vector(settings.external_chemical_count);
		fill(state.external_chemicals.begin(), state.external_chemicals.end(), settings.init_val);
		state.internal_chemicals = common_types::real_vector(settings.internal_chemical_count);
		fill(state.internal_chemicals.begin(), state.internal_chemicals.end(), settings.init_val);

		return state;
	}
}