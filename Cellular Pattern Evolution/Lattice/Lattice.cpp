#include "../Lattice/Headers/Lattice.h"
#include "../ObjectiveFunctions/Headers/ObjectiveFunctionColorDistance.h"
#include "../Main/Headers/Logger.h"
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

	void lattice::init_evaluator()
	{
		evaluator = make_unique<objective_functions::objective_func_color_dist>(
			"evaluator", 0.0, shared_from_this());
	}

	lattice::~lattice()
	{
		// so the compiler won't complain about the incomplete objective function type
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
	void lattice::simulate(function<void(phenotypes::phenotype&)> callback, bool observed_run)
	{
		statistics->reset();
		genotype->reset();
		phenotype->set_init_pattern(get_settings().init_pattern);

		if (observed_run) logger::get_logger().log_observed_run_start(get_statistics().gen_count);

		while (!genotype->get_criterion().should_stop(*this))
		{
			auto& cells = phenotype->expose_cells();
			//for (auto& cell : cells)
			//	genotype->get_controller().set_next_state(*cell);
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
			if (settings->allow_structure_change)
			{
				auto actions = phenotype->rearrange_topology();
				get_statistics().observed_run_stat.merge_count += actions.first;
				get_statistics().observed_run_stat.split_count += actions.second;
			}

			if (observed_run)
			{
				auto& ors = get_statistics().observed_run_stat;
				ors.cell_count = cells.size();
				if (!evaluator) init_evaluator();
				ors.color_distance = evaluator->eval();
				logger::get_logger().log_observed_run_gen(statistics->sim_eval_count + 1, ors);
			}

			statistics->sim_eval_count++;
		}

		if (observed_run) logger::get_logger().log_observed_run_end();
	}
}