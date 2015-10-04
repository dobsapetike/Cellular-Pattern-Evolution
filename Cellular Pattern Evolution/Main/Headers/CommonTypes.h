#ifndef COMMONTYPES_H
#define COMMONTYPES_H

#include <vector>

namespace common_types
{
	typedef std::vector<double> real_vector;

	enum direction 
	{
		upper,
		left,
		right,
		lower,
	};

	enum color_type
	{
		grayscale,
		rgb,
	};

	color_type parse_color_type(std::string type);
}

#endif