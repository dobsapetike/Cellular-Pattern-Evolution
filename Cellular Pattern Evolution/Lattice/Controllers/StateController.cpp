#include <algorithm>

#include "../Headers/Controllers/FeedForwardANNController.h"
#include "../Headers/Controllers/StateController.h"

namespace lattice
{
	namespace controllers
	{
		/**
			Controller factory method, constructing a controller instance based on settings
		*/
		unique_ptr<state_controller> create_controller(lattice_settings const& settings)
		{
			string type = settings.controller->FirstChildElement("Type")->GetText();
			transform(type.begin(), type.end(), type.begin(), ::tolower);

			if (type == "feedforwardanncontroller")
				return unique_ptr<feedforward_ann_controller>(new feedforward_ann_controller(settings));
			
			throw invalid_argument("Unknown controller type: " + type);
		}
	}
}
