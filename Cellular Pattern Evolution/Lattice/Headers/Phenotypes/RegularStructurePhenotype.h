#ifndef REGULAR_STRUCTURE_PHENOTYPE_H
#define REGULAR_STRUCTURE_PHENOTYPE_H

#include "Phenotype.h"

namespace lattice
{
	namespace phenotypes
	{
		/**
			Enum defining which type of neighbourhood is used
		*/
		enum neighbourhood_type
		{
			// all the 8 surrounding cells
			moore,
			// 4 orthogonally surrounding cells
			von_neumann,
		};

		/**
			Phenotype for the classic regular grid-shaped CA
		*/
		class regular_structure_phenotype : public phenotype
		{
		private:
			neighbourhood_type neighbourhood_type;
			// 2D array of cell pointers, topologically identical to the CA
			vector<vector<shared_ptr<lattice_cell>>> grid;
			// also maintain a single list of all cells for the getter
			vector<shared_ptr<lattice_cell>> cells;
		public:
			regular_structure_phenotype(lattice_settings const& settings);
			cell_type get_cell_type() const override { return regular; };
			// no split/merge possible
			merge_split_count rearrange_topology() override { return merge_split_count(0, 0); };
			void set_init_pattern(string pattern) override;
			vector<shared_ptr<lattice_cell>> const& expose_cells() const override { return cells; };
			neighbourhood get_neighbours(lattice_cell const& c) const override;
		};

		neighbourhood_type parse_neighbourhood_type(string type);
	}
}

#endif