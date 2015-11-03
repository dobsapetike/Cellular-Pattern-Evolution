#include "../Headers/Phenotypes/Phenotype.h"
#include "../Headers/Phenotypes/RegularStructurePhenotype.h"

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

			throw invalid_argument(
				"No matching phenotype found celltype: " + settings.cellType);
		}
	}
}