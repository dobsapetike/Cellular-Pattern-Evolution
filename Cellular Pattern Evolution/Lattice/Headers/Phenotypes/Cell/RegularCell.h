#include "Cell.h"
#include <functional>
#include <iostream>

namespace lattice
{
	namespace phenotypes
	{
		/**
			A simple cell in a classical grid-like CA
		*/
		class regular_cell : public lattice_cell
		{
		public:
			regular_cell(unsigned int x, unsigned int y, state_settings const& settings, 
				shared_ptr<regular_structure_phenotype> const& owner) : lattice_cell(x, y, settings, owner) { };
			/**
				Since there is no topology rearrangement in a regular CA, 
				there is no need for complatibility values
			*/
			virtual neighbourhood_compatibility get_neighbourhood_compatibility() const override 
			{
				throw invalid_argument("Compatibility: regular grid doesn't support split/merge!");
			};
		};
	}
}