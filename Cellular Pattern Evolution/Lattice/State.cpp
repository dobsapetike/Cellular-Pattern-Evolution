#include "Headers\State.h"
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

namespace lattice
{
	/**
		Initializes the states based on input settings
	*/
	state init_state(state_settings const& settings)
	{
		state state;

		state.external_chemicals = real_vector(settings.external_chemical_count);
		fill(state.external_chemicals.begin(), state.external_chemicals.end(), settings.init_val);
		state.internal_chemicals = real_vector(settings.internal_chemical_count);
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

		auto color = static_cast<unsigned char>(cl * 255);
		return rgb {
			color, color, color
		};
	}

	rgb color_level_to_rgb(real_vector const& cl)
	{
		if (cl.size() == 1)
			return color_level_to_rgb(cl[0]);
		if (cl.size() != 3)
			throw invalid_argument("Vector length mismatch at color level to RGB conversion! RGB has three values!");

		unsigned char vals[3];
		for (unsigned int i = 0; i < 3; ++i)
		{
			double d = cl[i];
			if (d < 0.0 || d > 1.0)
				throw invalid_argument("Color level should be a real value between 0 and 1!");
			vals[i] = static_cast<unsigned char>(d * 255);
		}
		return rgb {
			vals[0], vals[1], vals[2]
		};
	}

	rgb parse_rgb(std::string s)
	{
		// format: rgb(iR, iG, iB)
		// remove whitespace
		s.erase(std::remove_if(s.begin(), s.end(),
			[](char c){ return c == ' '; }), s.end());

		unsigned int start = s.find('(') + 1;
		unsigned int end = s.find(')');
		string cols = s.substr(start, end- start);

		replace(cols.begin(), cols.end(), ',', ' ');
		istringstream ist(cols);

		unsigned int r, g, b;
		ist >> r >> g >> b;
		
		return rgb { 
			static_cast<unsigned char>(r),
			static_cast<unsigned char>(g),
			static_cast<unsigned char>(b) 
		};
	}

	unsigned int compute_rgb_distance(rgb col1, rgb col2)
	{
		return abs(col1.r - col2.r) + abs(col1.g - col2.g) + abs(col1.b - col2.b);
	}
}