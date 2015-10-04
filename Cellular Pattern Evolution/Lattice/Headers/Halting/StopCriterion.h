#ifndef STOP_CRITERION_H
#define STOP_CRITERION_H

#include <tinyxml/tinyxml.h>

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
			// constructor receives the configuration XML, which is parsed in the subclasses
			stop_criterion(TiXmlElement init_config) { initialize(init_config); };
			virtual void initialize(TiXmlElement init_config);
			// TODO decides whether the simulation should stop
			virtual bool should_stop();
		};
	}
}

#endif