#include "../../Headers/Phenotypes/Cell/Cell.h"

namespace lattice
{
	namespace phenotypes
	{
		/**
			Returns a vector of neighbouring cells defined by the phenotype
		*/
		neighbourhood lattice_cell::get_neighbours() const
		{
			return _owner->get_neighbours(*this);
		}
	}
}