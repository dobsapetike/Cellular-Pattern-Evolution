#include "../Headers/Phenotypes/Phenotype.h"
#include "../Headers/Phenotypes/RegularStructurePhenotype.h"
#include "../Headers/Phenotypes/IrregularSquarePhenotype.h"
#include "../Headers/Phenotypes/IrregularRectanglePhenotype.h"

namespace lattice
{
	namespace phenotypes
	{
		/**
			Phenotype factory method, constructing a phenotype instance based on settings
		*/
		unique_ptr<phenotype> create_phenotype(lattice_settings const& settings)
		{
			if (settings.cellType == regular)
				return std::make_unique<regular_structure_phenotype>(settings);
			if (settings.cellType == irregular_square)
				return std::make_unique<irregular_square_phenotype>(settings);
			if (settings.cellType == irregular_rectangle)
				return std::make_unique<irregular_rectangle_phenotype>(settings);

			throw invalid_argument(
				"No matching phenotype found for celltype: " + settings.cellType);
		}
	}
}