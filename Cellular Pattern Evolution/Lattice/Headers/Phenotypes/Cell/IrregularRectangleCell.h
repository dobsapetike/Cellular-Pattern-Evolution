#ifndef IRREGULAR_RECT_CELL_H
#define IRREGULAR_RECT_CELL_H

#include "Cell.h"
#include "../IrregularRectanglePhenotype.h"

namespace lattice
{
	namespace phenotypes
	{
		/**
			A rectangular cell with unrestricted non-unit dimensions
		*/
		class irregular_rectangle_cell : public lattice_cell
		{
		protected:
			unsigned int width, height;
		public:
			irregular_rectangle_cell(
				unsigned int x, unsigned int y,
				unsigned int width, unsigned int height, state_settings const& settings,
				shared_ptr<irregular_rectangle_phenotype> const& owner)
				: lattice_cell(x, y, settings, owner), width(width), height(height)
			{
				vector<point> points{
					point(x, y), point(x, y + height),
					point(x + width, y + height), point(x + width, y), point(x, y)
				};
				geometry = make_polygon(points);
			};

			unsigned int get_width() const { return width; }
			unsigned int get_height() const { return height; }

			/**
				TODO
			*/
			virtual neighbourhood_compatibility get_neighbourhood_compatibility() const override
			{
				throw invalid_argument("Compatibility error: regular grid doesn't support split/merge!");
			};
		};
	}
}

#endif