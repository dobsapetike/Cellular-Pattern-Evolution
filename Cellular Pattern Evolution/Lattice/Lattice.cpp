#include "../Lattice/Headers/Lattice.h"

namespace lattice
{
	lattice::lattice(unique_ptr<lattice_settings> ls)
	{
		_settings = move(ls);
		_statistics = make_unique<lattice_statistics>();
		_phenotype = phenotypes::create_phenotype(get_settings());
		_genotype = make_unique<genotype::genotype>(get_settings());
	}

	/**
		Simulates the computation of the CA
	*/
	void lattice::simulate(function<void(phenotypes::phenotype&)> callback)
	{
		_statistics->reset();
		_genotype->reset();
		_phenotype->set_init_pattern(get_settings().init_pattern, get_settings().stateSettings);

		while (!_genotype->get_criterion().should_stop(*this))
		{
			auto cells = _phenotype->expose_cells();
			// compute the next state for each cell
			for (auto& cell : cells)
				_genotype->get_controller().set_next_state(*cell);
			// and make a simultaneous update
			for (auto& cell : cells)
				cell->apply_candidate();

			_statistics->eval_count++;
			// cout << "Eval num: " << _statistics->eval_count << endl;

			if (callback) callback(*_phenotype);
		}
	}
}