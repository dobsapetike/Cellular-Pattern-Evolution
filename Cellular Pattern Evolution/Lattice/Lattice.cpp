#include "../Lattice/Headers/Lattice.h"
#include <thread>

namespace lattice
{
	lattice::lattice(unique_ptr<lattice_settings> ls)
	{
		settings = move(ls);
		statistics = make_unique<lattice_statistics>();
		phenotype = phenotypes::create_phenotype(get_settings());
		genotype = make_unique<genotype::genotype>(get_settings());

		threads = make_unique<thread_pool>(
			max(thread::hardware_concurrency(), static_cast<unsigned int>(1))
		);
	}

	void lattice::update_cells(unsigned int order_number)
	{
		auto& cells = phenotype->expose_cells();
		unsigned int threadCount = min(threads->thread_count(), cells.size());
		// the number of cells for each thread
		unsigned int chunks = ceil(cells.size() / static_cast<double>(threadCount));

		unsigned int start = order_number * chunks;
		for (unsigned int cellindex = start; 
			cellindex < start + chunks && cellindex < cells.size(); 
			++cellindex)
		{
			genotype->get_controller().set_next_state(*cells[cellindex]);
		}
	}

	/**
		Simulates the computation of the CA
	*/
	void lattice::simulate(function<void(phenotypes::phenotype&)> callback)
	{
		statistics->reset();
		genotype->reset();
		phenotype->set_init_pattern();

		while (!genotype->get_criterion().should_stop(*this))
		{
			auto& cells = phenotype->expose_cells();
			//for (auto& cell : cells)
			//	_genotype->get_controller().set_next_state(*cell);
			for (unsigned int order = 0; order < threads->thread_count(); ++order)
			{
				threads->enqueue_job([&,order] () -> void { update_cells(order); });
			}
			threads->wait_for_jobs();
			// and make a simultaneous update
			for (auto& cell : cells)
			{
				cell->apply_candidate();
				if (callback) callback(*phenotype);
			}

			// also, update topology
			phenotype->rearrange_topology();

			statistics->sim_eval_count++;
			// cout << "Eval num: " << _statistics->eval_count << endl;
		}
	}
}