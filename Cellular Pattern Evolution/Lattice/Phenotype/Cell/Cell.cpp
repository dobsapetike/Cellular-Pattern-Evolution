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
			return owner->get_neighbours(*this);
		}

		/**
			Cell comparator for sorting
		*/
		function<bool(const lattice_cell&, const lattice_cell&)>
		cell_comparator = [](const lattice_cell& a, const lattice_cell& b) -> bool {
			return a.get_y() > b.get_y() ||
				(a.get_y() == b.get_y() && a.get_x() > b.get_x());
		};
	}
}