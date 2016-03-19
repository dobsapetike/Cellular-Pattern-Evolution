#include "../Headers/Lattice.h"
#include "../Headers/Halting/EvalNumberCriterion.h"

namespace lattice
{
	namespace halting
	{
		/**
			Read evalution limit from config settings
		*/
		eval_num_halting::eval_num_halting(lattice_settings const& settings)
		{
			step_limit = atoi(settings.stop_criterion->FirstChildElement("Limit")->GetText());
		}

		/*
			Simple haling criterion: stop after a predefined number of steps
		*/
		bool eval_num_halting::should_stop(lattice& lattice)
		{
			return lattice.get_statistics().sim_eval_count >= step_limit;
		}
	}
}