#ifndef PHENOTYPE_H
#define PHENOTYPE_H

#include "../LatticeSettings.h"
#include <vector>
#include <map>
#include <memory>

namespace lattice
{
	namespace phenotypes
	{
		class lattice_cell;
		typedef map<direction, vector<shared_ptr<lattice_cell>>> neighbourhood;

		/**
			Phenotype captures the observable characteristics of the individual,
			in our case the representation in the memory of the computer.
			This abstract class defines the common interface for different celltypes.
		*/
		class phenotype
		{
		private:
			unsigned int width, height;
			state_settings st_settings;
		public:
			phenotype(lattice_settings const& settings) :
				width(settings.width), height(settings.height), 
				st_settings(settings.stateSettings) { };

			unsigned int get_width() const { return width; }
			unsigned int get_height() const { return height; }
			state_settings const& get_state_settings() const { return st_settings; }

			virtual void rearrange_topology() = 0;
			virtual cell_type get_cell_type() const = 0;
			virtual void set_init_pattern() = 0;
			virtual vector<shared_ptr<lattice_cell>> const& expose_cells() const = 0;
			virtual neighbourhood get_neighbours(lattice_cell const& c) const = 0;
		};

		/**
			Phenotype factory method, constructing a phenotype instance based on settings
		*/
		unique_ptr<phenotype> create_phenotype(lattice_settings const& settings);
	}
}

#endif