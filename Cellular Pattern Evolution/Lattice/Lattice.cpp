#include "../Lattice/Headers/Lattice.h"
#include <thread>

namespace lattice
{
	lattice::lattice(unique_ptr<lattice_settings> ls)
	{
		_settings = move(ls);
		_statistics = make_unique<lattice_statistics>();
		_phenotype = phenotypes::create_phenotype(get_settings());
		_genotype = make_unique<genotype::genotype>(get_settings());

		_desired_thread_count = max(thread::hardware_concurrency(), static_cast<unsigned int>(1));
	}

	void lattice::update_cells(vector<shared_ptr<phenotypes::lattice_cell>>& cells,
		unsigned int chunks, unsigned int order_number)
	{
		unsigned int start = order_number * chunks;
		for (unsigned int i = start; i < start + chunks || i == cells.size() - 1; ++i)
		{
			_genotype->get_controller().set_next_state(*cells[i]);
		}
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

			// the number of cells for each thread
			unsigned int threadCount = min(_desired_thread_count, cells.size());
			unsigned int tw = ceil(cells.size() / static_cast<double>(threadCount));
			// compute the next state for each cell
			vector<thread> threads(threadCount);
			for (unsigned int i = 0; i < threadCount; ++i)
				threads[i] = thread(&lattice::update_cells, this, cells, tw, i);
			for (auto& t : threads) t.join();
			/*for (auto& cell : cells)
				_genotype->get_controller().set_next_state(*cell);*/

			// and make a simultaneous update
			for (auto& cell : cells)
			{
				cell->apply_candidate();
				if (callback) callback(*_phenotype);
			}

			_statistics->eval_count++;
			// cout << "Eval num: " << _statistics->eval_count << endl;
		}
	}
}