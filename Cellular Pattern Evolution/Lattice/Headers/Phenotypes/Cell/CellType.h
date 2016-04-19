#ifndef CELLTYPE_H
#define CELLTYPE_H

#include <string>

namespace lattice
{
	namespace phenotypes
	{
		/**
			Enum defining the possible cell types of the automaton
		*/
		enum cell_type
		{
			regular,
			irregular_square,
			irregular_rectangle,
			irregular_rectangle_strict,
			voronoi,
		};

		cell_type parse_cell_type(std::string type);
	}
}

#endif