#ifndef IRREGULAR_SQUARE_CELL_H
#define IRREGULAR_SQUARE_CELL_H

#include "IrregularRectangleCell.h"
#include "../IrregularSquarePhenotype.h"

namespace lattice
{
	namespace phenotypes
	{
		/**
			A square cell with unrestricted non-unit dimensions 
		*/
		class irregular_square_cell : public irregular_rectangle_cell
		{
		public:
			irregular_square_cell(
				unsigned int x, unsigned int y, 
				unsigned int dimension, state_settings const& settings,
				shared_ptr<irregular_square_phenotype> const& owner)
				: irregular_rectangle_cell(x, y, dimension, dimension, settings, owner)
			{ };
		};
	}
}

#endif