#include <algorithm>
#include "../../Headers/Phenotypes/Cell/CellType.h"

namespace lattice
{
	namespace phenotypes
	{
		cell_type parse_cell_type(std::string type)
		{
			std::transform(type.begin(), type.end(), type.begin(), ::tolower);
			if (type == "regular")				return regular;
			if (type == "irregularsquare")		return irregular_square;
			if (type == "irregularrectangle")	return irregular_rectangle;
			if (type == "voronoi")				return voronoi;

			throw std::invalid_argument("Unknown cell type: " + type);
		}
	}
}