#include <algorithm>
#include "../Headers/Halting/StopCriterion.h"
#include "../Headers/Halting/EvalNumberCriterion.h"

namespace lattice
{
	namespace halting
	{
		/**
			Criterion factory method, constructing a stop criterion instance based on settings
		*/
		unique_ptr<stop_criterion> create_stop_criterion(lattice_settings const& settings)
		{
			string type = settings.stop_criterion->FirstChildElement("Criterion")->GetText();
			transform(type.begin(), type.end(), type.begin(), ::tolower);

			if (type == "evalnums")
				return unique_ptr<eval_num_halting>(new eval_num_halting(settings));

			throw invalid_argument("Unknown stop criterion type: " + type);
		}
	}
}