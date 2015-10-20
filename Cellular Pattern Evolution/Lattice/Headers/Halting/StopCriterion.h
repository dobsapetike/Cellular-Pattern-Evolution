#ifndef STOP_CRITERION_H
#define STOP_CRITERION_H

#include <tinyxml/tinyxml.h>
#include "../LatticeStatistics.h"
#include "../LatticeSettings.h"

namespace lattice
{
	namespace halting
	{
		/*
			Abstract class for implementing the logic of the halting criterion
			of the simulation.
		*/
		class stop_criterion
		{
		public:
			// decides whether the simulation should stop
			virtual bool should_stop(lattice_statistics const& statistics) const = 0;
		};

		/**
			Criterion factory method, constructing a stop criterion instance based on settings
		*/
		unique_ptr<stop_criterion> create_stop_criterion(lattice_settings const& settings);
	}
}

#endif