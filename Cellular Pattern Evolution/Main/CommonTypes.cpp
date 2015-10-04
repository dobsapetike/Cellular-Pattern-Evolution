#include <algorithm>
#include <iostream>
#include <string>
#include "Headers/CommonTypes.h"

namespace common_types
{
	color_type parse_color_type(std::string type)
	{
		std::transform(type.begin(), type.end(), type.begin(), ::towlower);
		if (type == "rgb") return rgb;
		return grayscale;	// default
	}
}