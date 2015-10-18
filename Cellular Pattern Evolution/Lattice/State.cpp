#include "Headers\State.h"
#include <iostream>

using namespace std;

namespace lattice
{
	/**
		Initializes the states based on input settings
	*/
	state init_state(state_settings const& settings)
	{
		state state;

		state.external_chemicals = common_types::real_vector(settings.external_chemical_count);
		fill(state.external_chemicals.begin(), state.external_chemicals.end(), settings.init_val);
		state.internal_chemicals = common_types::real_vector(settings.internal_chemical_count);
		fill(state.internal_chemicals.begin(), state.internal_chemicals.end(), settings.init_val);

		state.color = {
			255, 255, 255
		};

		return state;
	}

	/**
		Getting the color level of grayscale/rgb values, returns rgb struct
	*/
	rgb color_level_to_rgb(double const& cl)
	{
		if (cl < 0.0 || cl > 1.0)
			throw invalid_argument("Color level should be a real value between 0 and 1!");

		int color = (int)(cl * 255);
		return rgb {
			color, color, color
		};
	}

	rgb color_level_to_rgb(common_types::real_vector const& cl)
	{
		if (cl.size() == 1)
			return color_level_to_rgb(cl[0]);
		if (cl.size() != 3)
			throw invalid_argument("Vector length mismatch at color level to RGB conversion! RGB has three values!");

		int vals[3];
		for (unsigned int i = 0; i < 3; ++i)
		{
			double d = cl[i];
			if (d < 0.0 || d > 1.0)
				throw invalid_argument("Color level should be a real value between 0 and 1!");
			vals[i] = (int)(d * 255);
		}
		return rgb {
			vals[0], vals[1], vals[2]
		};
	}
}