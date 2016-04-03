#ifndef VORONOI_CELL_H
#define VORONOI_CELL_H

#include "Cell.h"
#include "../VoronoiPhenotye.h"
#include "../Geometry/VoronoiDiagram.h"
#include "../../../../Main/Headers/Logger.h"
#include <unordered_set>

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
				Return compatibility based on the the projected line length on the common axis
			*/
			virtual neighbourhood_compatibility get_neighbour_compatibility() const override
			{
				neighbourhood n(get_neighbours());
				neighbourhood_compatibility nc;

				unordered_set<point, point_hash> my_points;
				for (auto p : geometry.outer()) my_points.insert(p);

				vector<double> common_points;
				for (unsigned int d = upper; d <= lower; ++d)
				{
					direction dir = static_cast<direction>(d);
					if (n[dir].size() == 0) continue;

					double dim_min(DBL_MAX), dim_max(DBL_MIN);
					bool dim_index = dir == ::left || dir == ::right ? 0 : 1;
					for (unsigned int i = 0; i < n[dir].size(); ++i) {
						common_points.clear();
						for (unsigned int pi = 0; pi < n[dir][i]->get_geometry().outer().size() - 1; ++pi) {
							point p = n[dir][i]->get_geometry().outer()[pi];
							if (my_points.find(p) != my_points.end()) 
								common_points.push_back(dim_index ? p.get<0>() : p.get<1>());
						}
						if (common_points.size() != 2) {
							// logger::get_logger().log_error("Voronoi compatibility error, neighbour size mismatch!");
							nc[dir][n[dir][i]] = 1.0;
							continue;
						}

						for (auto& cp : common_points) {
							dim_min = min(dim_min, cp);
							dim_max = max(dim_max, cp);
						}
						nc[dir][n[dir][i]] = abs(common_points[0] - common_points[1]);
					}
					for (auto ncs : nc[dir]) nc[dir][ncs.first] /= dim_max - dim_min;
				}
				return nc;
			}
		};
	}
}

#endif