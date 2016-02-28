#ifndef IRREGULAR_RECT_PHENOTYPE_H
#define IRREGULAR_RECT_PHENOTYPE_H

#include "Phenotype.h"

namespace lattice
{
	namespace phenotypes
	{
		class irregular_rectangle_cell;	// forward declare

		/**
			Used internally to represent the spacial spread of a cell in a 2D array
		*/
		struct cell_info
		{
			coord cell_coord;
			shared_ptr<irregular_rectangle_cell> cell;
		};

		/**
			Phenotype for an irregular CA containing retangular cells
		*/
		class irregular_rectangle_phenotype : public phenotype
		{
		private:
			vector<shared_ptr<lattice_cell>> get_distinct_cells_in_line(
				unsigned int start, unsigned int end, unsigned int constdim, bool vertical) const;
		protected:
			// 2D array of cell pointers, topologically identical to the CA
			vector<vector<cell_info>> grid;
			// also maintain a single list of all cells for the getter
			vector<shared_ptr<lattice_cell>> cells;

			/**
				Empties the cell collections
			*/
			void clear();
			/**
				Inserts given cell into the cell collection
			*/
			void assign_cell(shared_ptr<irregular_rectangle_cell> cell);

			/**
				Topological updates
			*/
			virtual void split(shared_ptr<irregular_rectangle_cell> cell);
			virtual void merge(shared_ptr<irregular_rectangle_cell> cell);
			bool irregular_rectangle_phenotype::merge_side(vector<shared_ptr<lattice_cell>>& neigh,
				shared_ptr<irregular_rectangle_cell> cell, bool right); 

			virtual void apply_pattern(string pattern);

			shared_ptr<irregular_rectangle_phenotype> self_ptr;
		public:
			irregular_rectangle_phenotype(lattice_settings const& settings);
			/**
				Interface implementation
			*/
			virtual cell_type get_cell_type() const override { return irregular_rectangle; };
			virtual void rearrange_topology() override;
			virtual void set_init_pattern(string pattern) override;
			virtual vector<shared_ptr<lattice_cell>> const& expose_cells() const override { return cells; };
			virtual neighbourhood get_neighbours(lattice_cell const& c) const override;
		};
	}
}

#endif