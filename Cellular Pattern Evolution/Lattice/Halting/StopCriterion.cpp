#include <algorithm>
#include "../Headers/Halting/StopCriterion.h"
#include "../Headers/Halting/EvalNumberCriterion.h"
#include "../Headers/Halting/EnergyCriterion.h"

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
				return std::make_unique<eval_num_halting>(settings);
			if (type == "energy")
				return std::make_unique<energy_halting>(settings);

			throw invalid_argument("Unknown stop criterion type: " + type);
		}
	}
}