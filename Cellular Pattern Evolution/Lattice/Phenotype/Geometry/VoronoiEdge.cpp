#include "../../../Main/Headers/CommonTypes.h"
#include "../../Headers/Phenotypes/Geometry/VoronoiEdge.h"

/**
	Gets the flipped version of this edge
*/
v_edge v_edge::flipped() const
{
	return v_edge{ x2, y2, x1, y1, flippable, !is_flipped };
}

/**
	Methods for wheter input edge is the follwer of this edge
*/
bool v_edge::follows_strict(const v_edge& e) const
{
	return float_equal(x2, e.x1) && float_equal(y2, e.y1);
}

bool v_edge::follows(const v_edge& e) const
{
	return follows_strict(e) ||
		(e.flippable && follows_strict(e.flipped()));
}

/**
	Equality operators
*/
bool v_edge::operator==(const v_edge& other) const
{
	return (float_equal(this->x1, other.x1) && float_equal(this->x2, other.x2)
		&& float_equal(this->y1, other.y1) && float_equal(this->y2, other.y2))
		|| (flippable && float_equal(this->x1, other.x2) && float_equal(this->x2, other.x1)
		&& float_equal(this->y1, other.y2) && float_equal(this->y2, other.y1));
}

bool v_edge::operator!=(const v_edge& other) const
{
	return !(*this == other);
}