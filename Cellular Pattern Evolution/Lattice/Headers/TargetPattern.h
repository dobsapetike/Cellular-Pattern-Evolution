#ifndef TARGETPATTERN_H
#define TARGETPATTERN_H

#include "State.h"

using namespace std;

namespace lattice
{
	/**
		Structure describing the target pattern as a raster -- NOT USED
	*/
	struct target_pattern
	{
	private:
		vector<vector<rgb>> pattern;
	public:
		// Constructor fills the inner structure based on a file
		target_pattern(unsigned int desired_width, unsigned int desired_height, string filepath);
		// Gets the element on the given position
		rgb at(unsigned int x, unsigned int y);
	};
}

#endif