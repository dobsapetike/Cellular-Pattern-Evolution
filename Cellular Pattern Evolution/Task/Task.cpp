#include "Headers/Task.h"
#include "../Main/Headers/Logger.h"
#include <fstream>
#include <boost/filesystem.hpp>

namespace task
{
	task::task(experiment const& exp)
	{
		// initialize fields based on experiment configuations
		try
		{
			_lattice = std::make_shared<lattice::lattice>(lattice::load_settings(
				exp.lattice_file, exp.controller_file, exp.stopcriterion_file));

			_obj_func = std::make_shared<objective_functions::ca_multiobj_func>(
				*objective_functions::load_settings(exp.objective_file),
				_lattice->get_genotype().get_controller().get_param_count(),
				_lattice
				);
			_optimizer = std::unique_ptr<optimizer::optimizer>(
				optimizer::create_optimizer(
					*optimizer::load_settings(exp.optimizer_file),
					_obj_func
				)
			);

			_experiment = std::make_unique<experiment>(exp);
		}
		catch (std::invalid_argument& ia)
		{
			logger::get_logger().log_error(ia.what());
		}
	}

	/**
		Executes the experiment
	*/
	void task::execute()
	{
		logger::get_logger().log_experiment_start(_experiment->name);

		// lazily initialize some of the helper instances
		_plotter = std::make_unique<plotter>();
		_painter = std::make_unique<painter>();

		_optimizer->init();
		unsigned int gen(0);
		// while stopping criterion is not met, call the optimizer stops
		while (!_optimizer->should_stop())
		{
			_optimizer->step();

			auto sol = _optimizer->get_solution();

			double min = DBL_MAX;
			for (unsigned int i = 0; i < sol.size(); ++i)
			{
				if (sol[i].second[0] < min)
				{
					min = sol[i].second[0];
					result = sol[i].first;
				}
			}

			_plotter->add_point(min);
			logger::get_logger().log_evol_stat("Fitness: " + to_string(min));

			_lattice->get_genotype().get_controller().set_params(result);
			_lattice->simulate();
			_painter->paint(_experiment->name, "gen" + to_string(++gen) + ".png", _lattice->get_phenotype());

			if (min == 0) break;
		}

		logger::get_logger().log_experiment_end();
	}

	/**
		Finalizes the experiment - should be called after the execution
	*/
	void task::finalize()
	{
		// simulate a run while taking a picture after each evaluation
		static int eval = 0;
		auto callback = [&](lattice::phenotypes::phenotype& p)
		{
			_painter->paint(_experiment->name + "_Result", "eval" + to_string(++eval) + ".png", p);
		};
		_lattice->simulate(callback);
		// and save the lattice configuration
		boost::filesystem::create_directory("results/");
		ofstream ofile;
		ofile.open("results/" + _experiment->name + ".txt");
		for (auto param : _lattice->get_genotype().get_controller().get_params())
		{
			ofile << param << endl;
		}
		ofile.close();
	}
}