#include "../Headers/Genotype/Genotype.h"

namespace lattice
{
	namespace genotype
	{
		genotype::genotype(lattice_settings const& settings)
		{
			// initialize fields from config
			_controller = controllers::create_controller(settings);
			_stop_criterion = halting::create_stop_criterion(settings);
		}
	}
}