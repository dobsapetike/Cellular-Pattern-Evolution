#ifndef VORONOI_H
#define VORONOI_H

#include "VoronoiEdge.h"
#include "../../../../Main/Headers/CommonTypes.h"

/**
	Structure describing a given region of the diagram
	with its seed and corresponding polygon
*/
struct voronoi_region
{
	voronoi_region(point g, polygon p) 
		: generator(g), polygon(p)
	{ }

	point generator;
	polygon polygon;
};

/**
	A factory wrapping the external Voronoi Diagram Generator for more convenient usage
	and compatibility reasons with the existing modules
*/
class voronoi_diagram_factory
{
private:
	float width, height;

	void place_side_vertice(float x, float y, std::vector<std::vector<float>>& vertices);
	v_edge construct_side_edge(direction dir, float start, float end);
public:
	/**
		Constructos the voronoi region based on the generator points and the size of the 
		working environment
	*/
	std::vector<std::shared_ptr<voronoi_region>> create_diagram(std::vector<point> const& points,
		float width, float height);
};

#endif