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
		typedef map<common_types::direction, vector<shared_ptr<lattice_cell>>> neighbourhood;

		/**
			Phenotype captures the observable characteristics of the individual,
			in our case the representation in the memory of the computer.
			This abstract class defines the common interface for different celltypes.
		*/
		class phenotype
		{
		public:
			virtual bool is_split_merge_allowed() const = 0;
			virtual const cell_type get_cell_type() const = 0;
			virtual std::vector<std::shared_ptr<lattice_cell>> expose_cells() const = 0;
			virtual neighbourhood get_neighbours(lattice_cell const& c) const = 0;
		};

		/**
			Phenotype factory method, constructing a phenotype instance based on settings
		*/
		std::unique_ptr<phenotype> create_phenotype(lattice_settings const& settings);
	}
}

#endif