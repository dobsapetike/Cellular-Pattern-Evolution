#include "Headers/Task.h"
#include "../Main/Headers/Logger.h"
#include <fstream>
#include <limits>
#include <boost/filesystem.hpp>

namespace task
{
	task::task(experiment const& exp)
	{
		// initialize fields based on experiment configuations
		try
		{
			lattice = std::make_shared<lattice::lattice>(lattice::load_settings(
				exp.lattice_file, exp.controller_file, exp.stopcriterion_file));

			obj_func = std::make_shared<objective_functions::ca_multiobj_func>(
				*objective_functions::load_settings(exp.objective_file),
				lattice->get_genotype().get_controller().get_param_count(),
				lattice
			);
			optimizer = std::unique_ptr<optimizer::optimizer>(
				optimizer::create_optimizer(
					*optimizer::load_settings(exp.optimizer_file),
					obj_func
				)
			);

			this->plotter = std::make_unique<::plotter>();
			this->painter = std::make_unique<::painter>();

			experiment_ptr = std::make_unique<experiment>(exp);
		}
		catch (std::invalid_argument& ia)
		{
			logger::get_logger().log_error(ia.what());
		}
	}

	bool task::running = false;
	BOOL WINAPI task::handle_abort(DWORD c_event)
	{
		if (!running) return true;
		// reset switch when ctrl+c has been pressed
		if (c_event == CTRL_C_EVENT)
		{
			logger::get_logger().log_info(
				"CTRL + C has been pressed, at the end of current generation the evolution stops!");
			running = false;
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
		//SetConsoleCtrlHandler(static_cast<PHANDLER_ROUTINE>(_handle_abort), true);
		running = true;

		logger::get_logger().log_experiment_start(experiment_ptr->name);

		optimizer->init();
		// while stopping criterion is not met, call the optimizer stops
		while (!optimizer->should_stop() && running)
		{
			optimizer->step();

			auto sol = optimizer->get_solution();

			for (unsigned int i = 0; i < sol.size(); ++i)
			{
				RealVector params = vector_convert(sol[i].first);
				obj_func->closestFeasible(params);

				double fitness = accumulate(sol[i].second.begin(), sol[i].second.end(), 0.0);
				if (fitness < result_fitness)
				{
					result_fitness = fitness;
					result = vector_convert(params);
				}
			}

			plotter->add_point(result_fitness);
			logger::get_logger().log_evol_stat("Fitness: " + to_string(result_fitness));

			lattice->get_genotype().get_controller().set_params(result);
			lattice->simulate();

			logger::get_logger().log_evol_stat("Ended after step: " + to_string(lattice->get_statistics().sim_eval_count));

			if (result_fitness == 0)
				break;

			painter->paint(experiment_ptr->name, "gen" + to_string(optimizer->step_count())
				+ ".png", lattice->get_phenotype());
		}

		running = false;
		logger::get_logger().log_experiment_end(experiment_ptr->name);
	}

	void task::simulate()
	{
		lattice->simulate();
		painter->paint("simulator", "sim.png", lattice->get_phenotype());
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
			this->painter->paint(experiment_ptr->name + "_Result", 
				"eval" + to_string(++eval) + "_" + to_string(lattice->get_statistics().sim_eval_count) + ".png", p);
		};
		lattice->get_genotype().get_controller().set_params(result);
		lattice->simulate(callback);

		// print the generation number on the images
		string command = "ImageUtil.exe " + experiment_ptr->name + "_Result";
		system(command.c_str());
		boost::filesystem::create_directory("results/");
		// create video
		command = "ffmpeg.exe -nostats -loglevel 0 -framerate 60 -i pics/" + experiment_ptr->name +
			"_Result/eval%d.png -c: libx264 -r 30 -pix_fmt yuv420p results/" + experiment_ptr->name + ".mp4";
		system(command.c_str());
		boost::filesystem::remove_all("pics/" + experiment_ptr->name + "_Result");
		// and save the lattice configuration
		ofstream ofile;
		ofile.open("results/" + experiment_ptr->name + ".txt");
		ofile.precision(numeric_limits<double>::max_digits10);
		for (auto param : result) //_lattice->get_genotype().get_controller().get_params())
		{
			ofile << fixed << param << " ";
		}
		ofile << endl << "Evalution count: " << lattice->get_statistics().eval_count << endl;
		ofile.close();
	}
}