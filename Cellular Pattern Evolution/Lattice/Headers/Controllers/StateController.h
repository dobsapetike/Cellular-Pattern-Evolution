#ifndef STATECONTROLLER_H
#define STATECONTROLLER_H

#include "../Phenotypes/Cell/Cell.h"
#include "../State.h"
#include "../../../Main/Headers/CommonTypes.h"

namespace lattice
{
	namespace controllers
	{
		/**
			Abstract base of the controller for encoding the transition rules
		*/
		class state_controller
		{
		protected:
			state_settings state_params;
		public:
			state_controller(state_settings settings) : state_params(settings) { };
			// getter and setter for the parameters
			// regardless of the controller type, the main structure of each controller is a
			// vector of real numbers - it's values are altered by the optimizer
			virtual common_types::real_vector get_params() const = 0;
			virtual void set_params(common_types::real_vector const& p) = 0;
			// sets the next state of the given cell
			virtual void set_next_state(phenotypes::lattice_cell& cell) const = 0;
		};

		/**
			Controller factory method, constructing a controller instance based on settings
		*/
		unique_ptr<state_controller> create_controller(lattice_settings const& settings);
	}
}

#endif