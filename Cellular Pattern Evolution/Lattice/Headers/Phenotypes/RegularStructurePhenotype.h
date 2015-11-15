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
			neighbourhood_type _neighbourhood_type;
			// 2D array of cell pointers, topologically identical to the CA
			vector<vector<shared_ptr<lattice_cell>>> _grid;
			// also maintain a single list of all cells for the getter
			vector<shared_ptr<lattice_cell>> _cells;
		public:
			regular_structure_phenotype(lattice_settings const& settings);
			bool is_split_merge_allowed() const override { return false; };
			cell_type get_cell_type() const override { return regular; };
			void set_init_pattern(string init_pattern, state_settings state_set) override;
			const vector<shared_ptr<lattice_cell>>& expose_cells() const override { return _cells; };
			neighbourhood get_neighbours(lattice_cell const& c) const override;
		};

		neighbourhood_type parse_neighbourhood_type(string type);
	}
}

#endif