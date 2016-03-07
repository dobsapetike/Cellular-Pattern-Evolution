#ifndef COMMONTYPES_H
#define COMMONTYPES_H

#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

typedef std::vector<double> real_vector;
typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>> polygon;
typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> point;

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

struct coord
{
	unsigned int x, y;
};

color_type parse_color_type(std::string type);

/**
	Given a list of its vertices, create a polygon 
*/
polygon make_polygon(std::vector<point> const& points);

/**
	Given a valid polygon, rearrenges its vertices in clockwise order
*/
void reorder_clockwise(polygon& p);

bool float_equal(float a, float b, float eps = .005);
bool point_equal(point a, point b, float eps = .005);

/**
	Hash code for the point type
*/
struct point_hash
{
	unsigned int operator()(const point& p) const
	{
		unsigned int h(0);
		boost::hash_combine(h, floor(p.get<0>()));
		boost::hash_combine(h, floor(p.get<1>()));
		return h;
	}
};

/**
	Point comparison
*/

namespace boost { namespace geometry { namespace model {

bool operator==(const point<double, 2, cs::cartesian>& a, const point<double, 2, cs::cartesian>& b);
bool operator!=(const point<double, 2, cs::cartesian>& a, const point<double, 2, cs::cartesian>& b);

} } }

#endif