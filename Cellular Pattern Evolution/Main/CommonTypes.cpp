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

/**
	Given a list of its vertices, create a polygon
*/
polygon make_polygon(std::vector<point> const& points)
{
	polygon poly;
	std::string ps("");

	std::ostringstream oss;
	for (unsigned int i = 0; i < points.size(); ++i)
	{
		auto& point = points[i];
		oss << point.get<0>() << " " << point.get<1>();
		if (i != points.size() - 1)
			oss << ",";
	}
	boost::geometry::read_wkt("POLYGON((" + oss.str() + "))", poly);

	return poly;
}

/**
	Given a valid polygon, rearrenges its vertices in clockwise order
*/
void reorder_clockwise(polygon& p)
{
	float sum(0.0);
	auto& points = p.outer();
	for (unsigned int i = 0; i < points.size() - 1; ++i)
	{
		sum += (points[i + 1].x() - points[i].x())
			* (points[i + 1].y() + points[i].y());
	}
	if (sum > 0) return;	// already clockwise
	boost::geometry::reverse(p);
}

bool float_equal(float a, float b, float eps)
{
	return fabs(a - b) < eps;
}

/**
	Point comparison
*/

bool point_equal(point a, point b, float eps)
{
	return float_equal(a.get<0>(), b.get<0>(), eps) 
		&& float_equal(a.get<1>(), b.get<1>(), eps);
}

namespace boost { namespace geometry { namespace model {

bool operator==(const point<double, 2, cs::cartesian>& a, const point<double, 2, cs::cartesian>& b)
{
	return point_equal(a, b);
}

bool operator!=(const point<double, 2, cs::cartesian>& a, const point<double, 2, cs::cartesian>& b)
{
	return !(a == b);
}

} } } 
