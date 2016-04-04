#ifndef VORONOI_EDGE_H
#define VORONOI_EDGE_H

#include <boost/functional/hash.hpp>

/**
Structure encapsulating an edge in the diagram
*/
struct v_edge
{
	// endpoints
	float x1, y1, x2, y2;
	// our edges are directed and their direction can be ambiguous
	bool flippable, is_flipped;

	/**
	Gets the flipped version of this edge
	*/
	v_edge flipped() const;

	/**
	Returns wheter input edge is the follwer of this edge
	*/
	bool follows_strict(const v_edge& e) const;
	bool follows(const v_edge& e) const;

	/**
	Equality operators
	*/
	bool operator==(const v_edge& other) const;
	bool operator!=(const v_edge& other) const;
};

/**
Hash code for the voronoi edge
*/
template <>
struct std::hash<v_edge>
{
	size_t operator()(const v_edge& edge) const
	{
		size_t h(0);
		boost::hash_combine(h, floor(edge.x1));
		boost::hash_combine(h, floor(edge.x2));
		boost::hash_combine(h, floor(edge.y1));
		boost::hash_combine(h, floor(edge.y2));
		return h;
	}
};

#endif
