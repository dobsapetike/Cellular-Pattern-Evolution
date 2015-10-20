#include "../Lattice/Headers/Lattice.h"
#include "../ObjectiveFunctions/Headers/CAMultiObjectiveFunction.h"

#include <iostream>

namespace lattice
{
	lattice::lattice(unique_ptr<lattice_settings> ls)
	{
		_settings = move(ls);
		_statistics = unique_ptr<lattice_statistics>(new lattice_statistics());
		_phenotype = phenotypes::create_phenotype(get_settings());
		_genotype = unique_ptr<genotype::genotype>(new genotype::genotype(get_settings()));
	}

	/**
		Simulates the computation of the CA
	*/
	void lattice::simulate()
	{
		_statistics->reset();
		_phenotype.get()->set_init_pattern(get_settings().init_pattern, get_settings().stateSettings);

		while (!_genotype->get_criterion().should_stop(*_statistics))
		{
			auto cells = _phenotype.get()->expose_cells();
			for (auto cell : cells)
			{
				_genotype.get()->get_controller().set_next_state(*cell);
			}

			_statistics->_eval_count++;
		}
	}
}