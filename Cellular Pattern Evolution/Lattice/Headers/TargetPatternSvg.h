#ifndef TARGETPATTERNSVG_H
#define TARGETPATTERNSVG_H

#include "State.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>

typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>> polygon;

using namespace std;

namespace lattice
{
	struct color_polygon
	{
		rgb color;
		polygon polygon;
	};

	/**
		Structure describing the target pattern as a list of polygons
	*/
	struct target_pattern_svg
	{
	private:
		vector<shared_ptr<color_polygon>> _polygons;
	public:
		vector<shared_ptr<color_polygon>> const& get_polygons() const { return _polygons; }
		// Constructor fills the inner structure based on a file
		target_pattern_svg(unsigned int desired_width, unsigned int desired_height, string filepath);
	};
}

#endif