#ifndef VORONOI_PHENOTYPE_H
#define VORONOI_PHENOTYPE_H

#include "Phenotype.h"
#include <boost/functional/hash.hpp>
#include "../Headers/Phenotypes/Geometry/VoronoiDiagram.h"

namespace lattice
{
	namespace phenotypes
	{
		class voronoi_cell;	// forward declare

		/**
			Phenotype for an irregular CA containing voronoi cells
		*/
		class voronoi_phenotype : public phenotype
		{
		private:
			// maintain a list of all cells for the getter
			vector<shared_ptr<lattice_cell>> cells;
			// remember which edge is incident with which region
			unordered_map<v_edge, vector<shared_ptr<voronoi_cell>>> incidents;
			// diagram creator
			voronoi_diagram_factory vor_factory;

			/**
				Topology updates
			*/
			void split(shared_ptr<voronoi_cell> cell, vector<point>& new_points);
			bool merge(shared_ptr<voronoi_cell> cell, point& new_point);
			bool is_feasible(point p);

			void place_incident(v_edge const& edge, shared_ptr<voronoi_cell> cell);
			void generate_topology(vector<point> const& points, 
				unordered_map<point, state, point_hash>* state_map = nullptr);
			void get_edge_directions(v_edge const& edge, bool* directions) const;

			shared_ptr<voronoi_phenotype> self_ptr;
		public:
			voronoi_phenotype(lattice_settings const& settings);
			/**
				Interface implementation
			*/
			virtual cell_type get_cell_type() const override { return voronoi; };
			virtual void rearrange_topology() override;
			virtual void set_init_pattern(string pattern) override;
			virtual vector<shared_ptr<lattice_cell>> const& expose_cells() const override { return cells; };
			virtual neighbourhood get_neighbours(lattice_cell const& c) const override;
		};
	}
}

#endif