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
			color_type color_type;
			// number of parameters for optimization
			unsigned int param_count;
		public:
			state_controller(state_settings settings) : state_params(settings) { };
			// getter and setter for the parameters
			// regardless of the controller type, the main structure of each controller is a
			// vector of real numbers - it's values are altered by the optimizer
			virtual real_vector get_params() const = 0;
			virtual void set_params(real_vector const& p) const = 0;
			virtual void compute_blurred_neighbour_external(phenotypes::lattice_cell& cell) const = 0;
			// sets the next state of the given cell
			virtual void set_next_state(phenotypes::lattice_cell& cell) const = 0;

			unsigned int get_param_count() const { return param_count; }
			::color_type get_color_type() const { return color_type; }
		};

		/**
			Controller factory method, constructing a controller instance based on settings
		*/
		unique_ptr<state_controller> create_controller(lattice_settings const& settings);
	}
}

#endif