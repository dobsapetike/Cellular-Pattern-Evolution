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

color_type parse_color_type(std::string type);
polygon make_polygon(std::vector<point> const& points);

#endif