#ifndef STATE_H
#define STATE_H

#include "../../Main/Headers/CommonTypes.h"

namespace lattice
{
	/**
		Structure for working with an rgb-encoded color
	*/
	struct rgb
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

	/**
		Enumeration of the structure changing actions
	*/
	enum action
	{
		merge,
		nothing,
		split,
	};

	/**
		Structure representing the state of a single CA cell.
		The state consists of two real vectors - called chemicals: 
			- internal used in the computation of the next state of the current cell
			- external used in the computation of the next state of the neighbouring cells
		Furthermore consist of an rgb value and a scalar representing the 
		cell color resp. the merge/split action
	*/
	struct state
	{
		real_vector internal_chemicals;
		real_vector external_chemicals;

		rgb color;
		action action;
	};

	/**
		Structure for describing state and state related properties.
		Used for storing config settings.
	*/
	struct state_settings
	{
		// chemical vector length
		unsigned int internal_chemical_count;
		unsigned int external_chemical_count;

		// initial chemical values
		double init_val;
	};

	/**
		Initializes the states based on input settings
	*/
	state init_state(state_settings const& settings);

	/**
		Getting the color level of grayscale/rgb values, returns rgb struct
	*/
	rgb color_level_to_rgb(double const& cl);
	rgb color_level_to_rgb(real_vector const& cl);

	rgb parse_rgb(std::string s);
	unsigned int compute_rgb_distance(rgb col1, rgb col2);
}

#endif