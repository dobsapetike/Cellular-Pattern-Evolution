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

		_threads = make_unique<thread_pool>(
			max(thread::hardware_concurrency(), static_cast<unsigned int>(1))
		);
	}

	void lattice::update_cells(unsigned int order_number)
	{
		auto cells = _phenotype->expose_cells();
		unsigned int threadCount = min(_threads->thread_count(), cells.size());
		// the number of cells for each thread
		unsigned int chunks = ceil(cells.size() / static_cast<double>(threadCount));

		unsigned int start = order_number * chunks;
		for (unsigned int cellindex = start; 
			cellindex < start + chunks || cellindex == cells.size() - 1; 
			++cellindex)
		{
			_genotype->get_controller().set_next_state(*cells[cellindex]);
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

		auto cells = _phenotype->expose_cells();
		while (!_genotype->get_criterion().should_stop(*this))
		{
			/*for (auto& cell : cells)
				_genotype->get_controller().set_next_state(*cell);*/
			for (unsigned int order = 0; order < _threads->thread_count(); ++order)
			{
				_threads->enqueue_job([&,order]{ update_cells(order); });
			}
			_threads->wait_for_jobs();
			
			// and make a simultaneous update
			for (auto& cell : cells)
			{
				cell->apply_candidate();
				if (callback) callback(*_phenotype);
			}

			_statistics->eval_count++;
			//cout << "Eval num: " << _statistics->eval_count << endl;
		}
	}
}