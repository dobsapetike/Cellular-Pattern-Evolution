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
			_eval_limit = atoi(settings.stop_criterion->FirstChildElement("Limit")->GetText());
		}

		/*
			Simple haling criterion: stop after a predefined number of steps
		*/
		bool eval_num_halting::should_stop(lattice_statistics const& statistics) const
		{
			return statistics._eval_count >= _eval_limit;
		}
	}
}