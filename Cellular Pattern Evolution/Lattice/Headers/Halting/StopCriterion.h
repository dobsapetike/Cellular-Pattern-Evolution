#ifndef STOP_CRITERION_H
#define STOP_CRITERION_H

#include <tinyxml/tinyxml.h>
#include "../LatticeSettings.h"

namespace lattice
{
	class lattice; // forward declaration of the lattice

	namespace halting
	{
		/*
			Abstract class for implementing the logic of the halting criterion
			of the simulation.
		*/
		class stop_criterion
		{
		protected:
			unsigned int step_limit = 200;
		public:
			// decides whether the simulation should stop
			virtual bool should_stop(lattice& statistics) = 0;
			// resets inner state befre simulation
			virtual void reset() = 0;

			unsigned int get_step_limit() const { return step_limit; }
		};

		/**
			Criterion factory method, constructing a stop criterion instance based on settings
		*/
		unique_ptr<stop_criterion> create_stop_criterion(lattice_settings const& settings);
	}
}

#endif