#include "Headers/Task.h"
#include "../Main/Headers/Logger.h"
#include <fstream>
#include <limits>
#include <boost/filesystem.hpp>

namespace task
{
	task::task(experiment const& exp, unsigned int num)
	{
		run_num = num;
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
		SetConsoleCtrlHandler(static_cast<PHANDLER_ROUTINE>(handle_abort), true);
		running = true;

		// set observable generation numbers
		unsigned int intervs = optimizer->step_count_limit() / observable_gen_count;
		vector<unsigned int> observable_gens;
		for (unsigned int i = 1; i < observable_gen_count; ++i)
			observable_gens.insert(observable_gens.begin(), i * intervs);
		observable_gens.insert(observable_gens.begin(), optimizer->step_count_limit());

		auto obj_names = obj_func->get_objectives();
		obj_names.push_back("Aggregate");
		logger::get_logger().log_experiment_start(experiment_name(), obj_names);

		optimizer->init();
		lattice->get_statistics().eval_count = 0;
		// while stopping criterion is not met, call the optimizer stops
		while (!optimizer->should_stop() && running)
		{
			optimizer->step();
			++lattice->get_statistics().gen_count;

			lattice::statistics_log log;
			log.generation = lattice->get_statistics().gen_count;
			log.eval_count = lattice->get_statistics().eval_count;
			log.objective_stats = vector<lattice::obj_func_stat>(
				obj_func->numberOfObjectives() + 1, lattice::obj_func_stat());

			auto sol = optimizer->get_solution();
			vector<double> fitn_all;
			fitn_all.reserve(sol.size());
			// aggregate fitness and start building the statistics
			for (unsigned int i = 0; i < sol.size(); ++i)
			{
				double fitness(0);
				unsigned int obj_index(0); 
				for (auto iter = sol[i].second.begin(); iter != sol[i].second.end(); ++iter) {
					fitness += *iter;
					log.objective_stats[obj_index].avg_fitn += *iter;
					log.objective_stats[obj_index].best_fitn = min(*iter, log.objective_stats[obj_index].best_fitn);
					++obj_index;
				}
				log.objective_stats.back().avg_fitn += fitness;
				fitn_all.push_back(fitness);

				if (fitness < result_fitness)
				{
					RealVector params = vector_convert(sol[i].first);
					obj_func->closestFeasible(params);
					result_fitness = fitness;
					result = vector_convert(params);
				}
			}
			log.objective_stats.back().best_fitn = result_fitness;

			// finish computing statistics
			for (auto& os : log.objective_stats) os.avg_fitn /= sol.size();
			for (unsigned int i = 0; i < sol.size(); ++i)
			{
				unsigned int obj_index(0);
				for (auto iter = sol[i].second.begin(); iter != sol[i].second.end(); ++iter) {
					log.objective_stats[obj_index].std_dev_fitn += 
						(*iter - log.objective_stats[obj_index].avg_fitn) * (*iter - log.objective_stats[obj_index].avg_fitn);
					++obj_index;
				}
				log.objective_stats.back().std_dev_fitn +=
					(fitn_all[i] - log.objective_stats.back().avg_fitn) * (fitn_all[i] - log.objective_stats.back().avg_fitn);
			}
			for (auto& os : log.objective_stats) os.std_dev_fitn = sqrt(os.std_dev_fitn / sol.size());

			// generate some output and move to the next generation
			plotter->add_point(result_fitness);
			logger::get_logger().log_info("Fitness: " + to_string(result_fitness));

			lattice->get_genotype().get_controller().set_params(result);

			bool observe = lattice->get_statistics().gen_count == observable_gens.back();
			if (observe) {
				observable_gens.pop_back();
				logger::get_logger().log_info("Running observed simulation!");
			}
			lattice->simulate(nullptr, observe);

			logger::get_logger().log_info("Ended after step: " + to_string(lattice->get_statistics().sim_eval_count));
			logger::get_logger().log_evol_stat(log);

			// if (result_fitness == 0) break;

			painter->paint(experiment_name(), "gen" + to_string(optimizer->step_count())
				+ ".png", lattice->get_phenotype());
		}

		running = false;
		logger::get_logger().log_experiment_end(experiment_name());

		lattice->get_genotype().get_controller().set_params(result);
	}

	void task::simulate()
	{
		lattice->simulate();
		painter->paint("simulator", "sim_" + experiment_name() +".png", lattice->get_phenotype());
	}

	static int eval;
	void task::generate_video()
	{
		// simulate a run while taking a picture after each evaluation
		logger::get_logger().log_info("Generating video of the simulation of the resulting automaton ... ");
		eval = 0;
		auto callback = [&](lattice::phenotypes::phenotype& p) {
			this->painter->paint(experiment_name() + "_Result",
				"eval" + to_string(++eval) + "_" + to_string(lattice->get_statistics().sim_eval_count) + ".png", p);
		};
		lattice->simulate(callback);

		// print the generation number on the images
		string command = "ImageUtil.exe " + experiment_name() + "_Result";
		system(command.c_str());
		// create video
		command = "ffmpeg.exe -nostats -loglevel 0 -framerate " + to_string(experiment_ptr->video_fps)
			+ " -i pics/" + experiment_name() + "_Result/eval%d.png"
			+ " -c: libx264 -r 30 -pix_fmt yuv420p results/" + experiment_name() + ".mp4";
		system(command.c_str());
		boost::filesystem::remove_all("pics/" + experiment_name() + "_Result");
		logger::get_logger().log_info("Video is ready! ");
	}

	/**
		Finalizes the experiment - should be called after the execution
	*/
	void task::finalize()
	{
		boost::filesystem::create_directory("results/");
		if (experiment_ptr->generate_video)
			generate_video();

		// and save the controller params
		ofstream ofile;
		ofile.open("results/" + experiment_name() + ".txt");
		ofile.precision(numeric_limits<double>::max_digits10);
		// the controller parameters
		// for (auto param : result)
		for (auto param : lattice->get_genotype().get_controller().get_params())
		{
			ofile << fixed << param << " ";
		}
		ofile << endl << endl;
		ofile.close();
	}
}