#include <algorithm>
#include <sstream>
#include <boost/geometry/io/wkt/wkt.hpp>
#include "Headers/CommonTypes.h"

color_type parse_color_type(std::string type)
{
	std::transform(type.begin(), type.end(), type.begin(), ::towlower);
	if (type == "rgb") return rgb;
	return grayscale;	// default
}

polygon make_polygon(std::vector<point> const& points)
{
	polygon poly;
	std::string ps("");

	std::ostringstream oss;
	for (unsigned int i = 0; i < points.size(); ++i)
	{
		auto point = points[i];
		oss << point.get<0>() << " " << point.get<1>();
		if (i != points.size() - 1)
			oss << ",";
	}
	boost::geometry::read_wkt("POLYGON((" + oss.str() + "))", poly);
		
	return poly;
}