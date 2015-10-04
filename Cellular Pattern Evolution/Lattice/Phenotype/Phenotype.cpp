#include "../Headers/Phenotypes/Phenotype.h"
#include "../Headers/Phenotypes/RegularStructurePhenotype.h"
#include "../Headers/Phenotypes/Cell/Cell.h"

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
				return unique_ptr<regular_structure_phenotype>(new regular_structure_phenotype(settings));

			throw invalid_argument(
				"No matching phenotype found celltype: " + settings.cellType);
		}
	}
}