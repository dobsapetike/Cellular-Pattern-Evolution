#ifndef CELL_H
#define CELL_H

#include <unordered_map>
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
		typedef unordered_map<direction, vector<shared_ptr<lattice_cell>>> neighbourhood;
		typedef unordered_map<direction, unordered_map<shared_ptr<lattice_cell>, double>> neighbourhood_compatibility;

		/**
			Abstract class for creating the basis of the the different 
			CA cell logic implementations
		*/
		class lattice_cell
		{
		private:
			// a pointer to the phenotype to obtain the neighbouring cells
			shared_ptr<phenotype> owner;
		protected:
			state cell_state, next_state;
			polygon geometry;
			// spacial coordinates of the cell
			unsigned int coord_x, coord_y;	// !!! should not be used during state computation
		public:
			lattice_cell(
				int x, int y, 
				state_settings const& state_settings,
				shared_ptr<phenotype> const& owner) 
					: owner(owner), cell_state(init_state(state_settings)), 
					  coord_x(x), coord_y(y) { };

			// neighbourhood functions
			neighbourhood get_neighbours() const;
			virtual neighbourhood_compatibility get_neighbour_compatibility() const = 0;

			// field getters
			unsigned int get_x() const { return coord_x; }
			unsigned int get_y() const { return coord_y; }
			state const& get_state() const { return cell_state; }
			polygon const& get_geometry() const { return geometry; }
			shared_ptr<phenotype> get_owner() { return owner; }
			// field setters
			void set_state(state const& s) { cell_state = move(s); }
			void next_candidate(state const& s) { next_state = s; }
			void apply_candidate() { cell_state = next_state; }

			void reset_action() { cell_state.action = nothing; }
			void reset_action(action a) { cell_state.action = a; }
		};

		/**
			Cell comparator for sorting
		*/
		extern function <bool(const lattice_cell&, const lattice_cell&)> cell_comparator;
	}
}

#endif