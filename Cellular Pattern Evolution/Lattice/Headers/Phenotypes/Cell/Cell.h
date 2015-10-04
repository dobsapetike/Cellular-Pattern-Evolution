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
		typedef map<common_types::direction, vector<shared_ptr<lattice_cell>>> neighbourhood;
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
			state cell_state;
			// spacial coordinates of the cell
			unsigned int coord_x, coord_y;	// !!! should not be used during state computation
		public:
			lattice_cell(
				unsigned int x, unsigned int y, 
				state_settings const& state_settings,
				shared_ptr<phenotype> const& owner) 
					: coord_x(x), coord_y(y), _owner(owner), 
					  cell_state(init_state(state_settings)) { };

			// neighbourhood functions
			neighbourhood get_neighbours() const;
			virtual neighbourhood_compatibility get_neighbourhood_compatibility() const = 0;

			// field getters
			const unsigned int get_x() const { return coord_x; }
			const unsigned int get_y() const { return coord_y; }
			state const& get_state() const   { return cell_state; }
			// field setters
			void set_state(state const& s) { cell_state = s; }
		};
	}
}

#endif