#include "../Headers/Genotype/Genotype.h"

namespace lattice
{
	namespace genotype
	{
		genotype::genotype(lattice_settings const& settings)
		{
			// initialize fields from config
			controller = controllers::create_controller(settings);
			stop_criterion = halting::create_stop_criterion(settings);
		}

		void genotype::reset()
		{
			stop_criterion->reset();
		}
	}
}