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

	bool task::_running = false;
	BOOL WINAPI task::_handle_abort(DWORD c_event)
	{
		if (!_running) return true;
		// reset switch when ctrl+c has been pressed
		if (c_event == CTRL_C_EVENT)
		{
			logger::get_logger().log_info(
				"CTRL + C has been pressed, at the end of current generation the evolution stops!");
			_running = false;
			return true;
		}
		return false;
	}

	/**
		Executes the experiment
	*/
	void task::execute()
	{
		// subscribe for the break event
		SetConsoleCtrlHandler(static_cast<PHANDLER_ROUTINE>(_handle_abort), true);
		_running = true;

		logger::get_logger().log_experiment_start(_experiment->name);

		// lazily initialize some of the helper instances
		_plotter = std::make_unique<plotter>();
		_painter = std::make_unique<painter>();

		_optimizer->init();
		// while stopping criterion is not met, call the optimizer stops
		while (!_optimizer->should_stop() && _running)
		{
			_optimizer->step();

			auto sol = _optimizer->get_solution();

			for (unsigned int i = 0; i < sol.size(); ++i)
			{
				RealVector params = vector_convert(sol[i].first);
				_obj_func->closestFeasible(params);

				if (sol[i].second[0] < _result_fitness)
				{
					_result_fitness = sol[i].second[0];
					_result = vector_convert(params);
				}
			}

			_plotter->add_point(_result_fitness);
			logger::get_logger().log_evol_stat("Objective: " + to_string(_result_fitness));

			_lattice->get_genotype().get_controller().set_params(_result);
			_lattice->simulate();

			_painter->paint(_experiment->name, "gen" + to_string(_optimizer->step_count()) 
				+ ".png", _lattice->get_phenotype());
		}

		_running = false;
		logger::get_logger().log_experiment_end();
	}

	/**
		Finalizes the experiment - should be called after the execution
	*/
	void task::finalize()
	{
		// simulate a run while taking a picture after each evaluation
		logger::get_logger().log_info("Generating video of the simulation of the resulting automaton ... ");
		static int eval = 0;
		auto callback = [&](lattice::phenotypes::phenotype& p)
		{
			_painter->paint(_experiment->name + "_Result", "eval" + to_string(++eval) + ".png", p);
		};
		_lattice->simulate(callback);

		boost::filesystem::create_directory("results/");
		// create video
		string command = "ffmpeg.exe -nostats -loglevel 0 -framerate 60 -i pics/" + _experiment->name + 
			"_Result/eval%d.png -c: libx264 -r 30 -pix_fmt yuv420p results/" + _experiment->name + ".mp4";
		system(command.c_str());
		boost::filesystem::remove_all("pics/" + _experiment->name + "_Result");
		// and save the lattice configuration
		ofstream ofile;
		ofile.open("results/" + _experiment->name + ".txt");
		for (auto param : _lattice->get_genotype().get_controller().get_params())
		{
			ofile << param << endl;
		}
		ofile.close();
	}
}