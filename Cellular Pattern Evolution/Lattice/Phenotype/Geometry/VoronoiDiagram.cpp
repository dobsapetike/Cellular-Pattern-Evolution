#include <unordered_set>
#include "../../Headers/Phenotypes/Geometry/VoronoiDiagram.h"
#include "../../Headers/Phenotypes/Geometry/VoronoiDiagramGenerator.h"

#define M_PI  3.141592653589793
#define M_2PI 2 * M_PI

using namespace std;

/**
	Constructos the voronoi region based on the generator points and the size of the
	working environment
*/
vector<shared_ptr<voronoi_region>> voronoi_diagram_factory::create_diagram(
	vector<point> const& points,
	float width, float height)
{
	this->width = width;
	this->height = height;

	vector<shared_ptr<voronoi_region>> result;

	unordered_set<v_edge> edges;
	vector<vector<float>> side_vertices(4, vector<float>());

	// use the generator to obtain edges
	VoronoiDiagramGenerator vg;
	vector<float> x(points.size()), y(points.size());
	for (unsigned int i = 0; i < points.size(); ++i) {
		x[i] = static_cast<float>(points[i].get<0>());
		y[i] = static_cast<float>(points[i].get<1>());
	}
	vg.generateVoronoi(&x[0], &y[0], x.size(), 0.0, width, 0.0, height);

	// wrap them in our edge object
	float x1, x2, y1, y2;
	vg.resetIterator();
	while (vg.getNext(x1, y1, x2, y2)) {
		if (float_equal(x1, x2) && float_equal(y1, y2)) continue;

		if (edges.insert(v_edge{ x1, y1, x2, y2, false, false }).second)
			place_side_vertice(x1, y1, side_vertices);
		if (edges.insert(v_edge{ x2, y2, x1, y1, false, false }).second)
			place_side_vertice(x2, y2, side_vertices);
	}

	// also append the side edges
	for (unsigned int dir = upper; dir <= lower; ++dir)
	{
		if (!side_vertices[dir].size()) {
			edges.insert(construct_side_edge(static_cast<direction>(dir), 0,
				dir == upper || dir == lower ? width : height));
			continue;
		}
		sort(side_vertices[dir].begin(), side_vertices[dir].end());
		for (unsigned int i = 0; i < side_vertices[dir].size(); ++i)
		{
			edges.insert(construct_side_edge(static_cast<direction>(dir),
				i == 0 ? 0 : side_vertices[dir][i - 1], side_vertices[dir][i]));
		}
		edges.insert(construct_side_edge(static_cast<direction>(dir), side_vertices[dir].back(),
			dir == upper || dir == lower ? width : height));
	}

	// create polygons by traversing them via the edges
	vector<polygon> regions;
	while (edges.size())
	{
		v_edge start;
		for (auto iter = edges.begin();; ++iter)
		{
			if (iter->flippable) continue;
			start = *iter;
			break;
		}

		polygon poly;
		v_edge current = start, temp = {};
		do
		{
			boost::geometry::append(poly, point(current.x2, current.y2));

			float max_angle = FLT_MIN;
			float dx(current.x2 - current.x1), dy(current.y2 - current.y1);
			float ctan = atan2(dy, dx);
			for (auto iter = edges.begin(); iter != edges.end(); ++iter)
			{
				if (*iter == current) continue;
				if (!current.follows(*iter)) continue;
				v_edge foll = current.follows_strict(*iter) ? *iter : iter->flipped();

				float dix(foll.x2 - foll.x1), diy(foll.y2 - foll.y1);
				float angle = atan2(diy, dix) - ctan;	// angle between the previous and current edge

				angle = fmod(angle, M_2PI);
				if (angle < 0) angle += M_2PI;
				if (angle > M_PI || float_equal(angle, M_PI)) continue;

				// since the polygon is convex (by definition) choose follower with biggest angle between
				if (angle > max_angle)
				{
					temp = foll;
					max_angle = angle;
				}
			}
			current = temp;
			edges.erase(current.is_flipped ? current.flipped() : current);

		} 
		while (current != start);

		// finally find the corresponding generator
		boost::geometry::append(poly, point(current.x2, current.y2));
		reorder_clockwise(poly);
		regions.push_back(poly);

		point seed;
		for (unsigned int i = 0; i < x.size(); ++i)
		{
			point pc(x[i], y[i]);
			if (boost::geometry::covered_by(pc, poly)) {
				seed = pc;
				break;
			}
		}

		result.push_back(make_shared<voronoi_region>(seed, poly));
	}

	return result;
}

void voronoi_diagram_factory::place_side_vertice(float x, float y,
	vector<vector<float>>& vertices)
{
	if (y == 0 && x > 0) vertices[lower].push_back(x);
	else if (y == height && x < width) vertices[upper].push_back(x);
	else if (x == 0 && y > 0) vertices[::left].push_back(y);
	else if (x == width && y < height) vertices[::right].push_back(y);
}

v_edge voronoi_diagram_factory::construct_side_edge(direction dir, float start, float end)
{
	v_edge e;
	switch (dir)
	{
	case lower:
		e.y1 = e.y2 = 0;
		e.x1 = start; e.x2 = end;
		break;
	case upper:
		e.y1 = e.y2 = height;
		e.x1 = start; e.x2 = end;
		break;
	case ::left:
		e.x1 = e.x2 = 0;
		e.y1 = start; e.y2 = end;
		break;
	case ::right:
		e.x1 = e.x2 = width;
		e.y1 = start; e.y2 = end;
		break;
	}
	e.flippable = true;
	e.is_flipped = false;
	return e;
}