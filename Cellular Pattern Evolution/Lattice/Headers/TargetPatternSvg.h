#ifndef TARGETPATTERNSVG_H
#define TARGETPATTERNSVG_H

#include "State.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>

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
		rgb background;
		vector<shared_ptr<color_polygon>> polygons;
	public:
		rgb const& get_backcolor() const { return background; }
		vector<shared_ptr<color_polygon>> const& get_polygons() const { return polygons; }
		// Constructor fills the inner structure based on a file
		target_pattern_svg(unsigned int desired_width, unsigned int desired_height, string filepath);
	};
}

#endif