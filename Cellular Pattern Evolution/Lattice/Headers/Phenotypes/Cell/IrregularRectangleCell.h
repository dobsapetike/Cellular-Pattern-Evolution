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
				int width, int height, state_settings const& settings,
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
				Return compatibility based on the the shared line length
			*/
			virtual neighbourhood_compatibility get_neighbour_compatibility() const override
			{
				neighbourhood n(get_neighbours());
				neighbourhood_compatibility nc;

				auto comp_shared = [](unsigned int as, unsigned int ae, unsigned int bs, unsigned int be) {
					return min(be, ae) - max(bs, as);
				};

				for (unsigned int d = upper; d <= lower; ++d)
				{
					direction dir = static_cast<direction>(d);
					if (n[dir].size() == 0) continue;
					for (unsigned int index = 0; index < n[dir].size(); ++index)
					{
						auto cell = static_pointer_cast<irregular_rectangle_cell>(n[dir][index]);
						// calculate shared line length and normalize it
						nc[dir][n[dir][index]] = dir == ::left || dir == ::right
							? comp_shared(get_y(), get_y() + get_height(),
							cell->get_y(), cell->get_y() + cell->get_height()) / static_cast<double>(get_height())
							: comp_shared(get_x(), get_x() + get_width(),
							cell->get_x(), cell->get_x() + cell->get_width()) / static_cast<double>(get_width());
					}
				}
				return nc;
			}
		};
	}
}

#endif