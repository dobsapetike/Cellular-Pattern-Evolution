#include "../Headers/Genotype/Genotype.h"

namespace lattice
{
	namespace genotype
	{
		genotype::genotype(lattice_settings const& settings)
		{
			_controller = controllers::create_controller(settings);
		}
	}
}