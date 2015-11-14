#ifndef CELL_H
#define CELL_H

#include <tuple>
#include <map>
#include <memory>

#include "../Phenotype.h"
#include "../../State.h"

using namespace std;

namespace lattice
{
	namespace phenotypes
	{
		/** 
			Types for working with the neighbourhood
		*/
		class lattice_cell;		// forward declaration
		typedef map<direction, vector<shared_ptr<lattice_cell>>> neighbourhood;
		typedef vector<pair<shared_ptr<lattice_cell>, double>> neighbourhood_compatibility;

		/**
			Abstract class for creating the basis of the the different 
			CA cell logic implementations
		*/
		class lattice_cell
		{
		private:
			// a pointer to the phenotype to obtain the neighbouring cells
			shared_ptr<phenotype> _owner;
		protected:
			state cell_state, next_state;
			polygon geometry;
			// spacial coordinates of the cell
			unsigned int coord_x, coord_y;	// !!! should not be used during state computation
		public:
			lattice_cell(
				unsigned int x, unsigned int y, 
				state_settings const& state_settings,
				shared_ptr<phenotype> const& owner) 
					: _owner(owner), cell_state(init_state(state_settings)), 
					  coord_x(x), coord_y(y) { };

			// neighbourhood functions
			neighbourhood get_neighbours() const;
			virtual neighbourhood_compatibility get_neighbourhood_compatibility() const = 0;

			// field getters
			unsigned int get_x() const { return coord_x; }
			unsigned int get_y() const { return coord_y; }
			state const& get_state() const { return cell_state; }
			polygon const& get_geometry() const { return geometry; }
			// field setters
			void set_state(state const& s) { cell_state = s; }
			void next_candidate(state const& s) { next_state = s; }
			void apply_candidate() { cell_state = next_state; }
		};
	}
}

#endif