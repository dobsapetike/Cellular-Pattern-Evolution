#ifndef VORONOI_CELL_H
#define VORONOI_CELL_H

#include "Cell.h"
#include "../VoronoiPhenotye.h"
#include "../Geometry/VoronoiDiagram.h"

namespace lattice
{
	namespace phenotypes
	{
		/**
			Cell describing a region in a Voronoi CA
		*/
		class voronoi_cell : public lattice_cell
		{
		private:
			shared_ptr<voronoi_region> region;
		public:
			voronoi_cell(
				shared_ptr<voronoi_region> voronoi_region,
				state_settings const& settings, shared_ptr<voronoi_phenotype> const& owner) 
				: lattice_cell(voronoi_region->generator.get<0>(), 
							   voronoi_region->generator.get<1>(), 
							   settings, owner), region(voronoi_region)
			{
				geometry = voronoi_region->polygon;
			};

			shared_ptr<voronoi_region> get_region() const { return region; }

			/**
				Return compatibility based on the the shared line length
				TODO
			*/
			virtual neighbourhood_compatibility get_neighbour_compatibility() const override
			{
				neighbourhood n(get_neighbours());
				neighbourhood_compatibility nc;
				for (unsigned int d = upper; d <= lower; ++d)
				{
					direction dir = static_cast<direction>(d);
					if (n[dir].size() == 0) continue;
					for (unsigned int i = 0; i < n[dir].size(); ++i)
						nc[dir][n[dir][i]] = 1.0;
				}
				return nc;
			}
		};
	}
}

#endif