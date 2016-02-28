#ifndef REGULAR_H
#define REGULAR_H

#include "Cell.h"

namespace lattice
{
	namespace phenotypes
	{
		/**
			A simple cell in a classical grid-like CA
		*/
		class regular_cell : public lattice_cell
		{
		public:
			regular_cell(unsigned int x, unsigned int y, state_settings const& settings, 
				shared_ptr<regular_structure_phenotype> const& owner) : lattice_cell(x, y, settings, owner)
			{
				vector<point> points { 
					point(x, y), point(x, y + 1), point(x + 1, y + 1), point(x + 1, y), point(x, y) 
				};
				geometry = make_polygon(points);
			};

			/**
				Since there is no topology rearrangement in a regular CA,
				there is no need for complatibility values
			*/
			virtual neighbourhood_compatibility get_neighbour_compatibility() const override
			{
				neighbourhood n(get_neighbours());
				neighbourhood_compatibility nc;
				for (unsigned int d = upper; d <= lower; ++d)
				{
					direction dir = static_cast<direction>(d);
					if (n[dir].size() == 0) continue;
					// only one neighbour can exist, so give it max compatibility
					nc[dir][n[dir][0]] = 1.0;
				}
				return nc;
			}
		};
	}
}

#endif