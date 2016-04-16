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

			experiment_ptr = std::make_unique<experiment>(exp);

			this->painter = std::make_unique<::painter>();
			this->plotter = experiment_ptr->plot ? std::make_unique<::plotter>() : nullptr;
		}
		catch (std::invalid_argument& ia)
		{
			logger::get_logger().log_error(ia.what());
		}
	}

	bool task::save = false;
	bool task::running = false;
	BOOL WINAPI task::handle_abort(DWORD c_event)
	{
		if (!running) return true;
		// reset switch when ctrl+c has been pressed
		if (c_event == CTRL_C_EVENT)
		{
			logger::get_logger().log_info(
				"CTRL + C has been pressed, at the end of current generation the evolution stops!");
			cout << "Would you like to save your progress? No results in early finealization! [y/n]" << endl;
			string opt;
			cin >> opt;
			save = opt == "y";
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
		running = true, save = false;

		// set observable generation numbers
		unsigned int observable_gen_count = min(experiment_ptr->observed_runs, optimizer->step_count_limit());
		unsigned int intervs = optimizer->step_count_limit() / observable_gen_count;
		vector<unsigned int> observable_gens;
		for (unsigned int i = 1; i < observable_gen_count; ++i)
			if (i * intervs > optimizer->step_count())
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

			pareto_sol = optimizer->get_solution();
			vector<double> fitn_all;
			fitn_all.reserve(pareto_sol.size());
			// aggregate fitness and start building the statistics
			for (unsigned int i = 0; i < pareto_sol.size(); ++i)
			{
				double fitness(0.0);
				unsigned int obj_index(0); 
				for (auto iter = pareto_sol[i].second.begin(); iter != pareto_sol[i].second.end(); ++iter) {
					fitness += *iter;
					log.objective_stats[obj_index].avg_fitn += *iter;
					log.objective_stats[obj_index].best_fitn = min(*iter, log.objective_stats[obj_index].best_fitn);
					++obj_index;
				}
				log.objective_stats.back().avg_fitn += fitness;
				fitn_all.push_back(fitness);

				if (fitness < result_fitness)
				{
					RealVector params = vector_convert(pareto_sol[i].first);
					obj_func->closestFeasible(params);
					result_fitness = fitness;
					result_params = vector_convert(params);
				}
			}
			log.objective_stats.back().best_fitn = *min_element(fitn_all.begin(), fitn_all.end());

			// finish computing statistics
			for (auto& os : log.objective_stats) os.avg_fitn /= pareto_sol.size();
			for (unsigned int i = 0; i < pareto_sol.size(); ++i)
			{
				unsigned int obj_index(0);
				for (auto iter = pareto_sol[i].second.begin(); iter != pareto_sol[i].second.end(); ++iter) {
					log.objective_stats[obj_index].std_dev_fitn += 
						(*iter - log.objective_stats[obj_index].avg_fitn) * (*iter - log.objective_stats[obj_index].avg_fitn);
					++obj_index;
				}
				log.objective_stats.back().std_dev_fitn +=
					(fitn_all[i] - log.objective_stats.back().avg_fitn) * (fitn_all[i] - log.objective_stats.back().avg_fitn);
			}
			for (auto& os : log.objective_stats) os.std_dev_fitn = sqrt(os.std_dev_fitn / pareto_sol.size());

			// generate some output and move to the next generation
			if (plotter) plotter->add_point(log.objective_stats.back().best_fitn);
			logger::get_logger().log_info("Fitness: " + to_string(log.objective_stats.back().best_fitn));

			lattice->get_genotype().get_controller().set_params(result_params);

			bool observe = lattice->get_statistics().gen_count == observable_gens.back();
			if (observe) {
				observable_gens.pop_back();
				logger::get_logger().log_info("Running observed simulation!");
			}
			lattice->simulate(nullptr, observe);

			logger::get_logger().log_info("Ended after step: " + to_string(lattice->get_statistics().sim_eval_count));
			logger::get_logger().log_evol_stat(log);

			if (result_fitness == 0) {
				logger::get_logger().log_info("Fitness is optimal, evolution terminates!");
				break;
			}

			painter->paint("gen" + to_string(optimizer->step_count())
				+ ".png", lattice->get_phenotype(), default_pic_path + experiment_name());
		}

		running = false;
		logger::get_logger().log_experiment_end(experiment_name());
		lattice->get_genotype().get_controller().set_params(result_params);
		if (save) 
		{
			logger::get_logger().log_info("Serialization of the current state has been started!");
			optimizer->serialize();
		}
	}

	void task::simulate(string result_folder, string file, bool observed)
	{
		if (!file.length()) file = "sim_" + experiment_name() + ".png";
		lattice->simulate(nullptr, observed);
		painter->paint(file, lattice->get_phenotype(), result_folder);
	}

	static int eval;
	void task::generate_video()
	{
		// simulate a run while taking a picture after each evaluation
		logger::get_logger().log_info("Generating video of the simulation of the resulting automaton ... ");
		eval = 0;
		auto callback = [&](lattice::phenotypes::phenotype& p) {
			for (unsigned int i = 0; i < (experiment_ptr->rich_video ? 1 : 3); ++i)
				this->painter->paint("eval" + to_string(++eval) + "_" + 
					to_string(lattice->get_statistics().sim_eval_count) + ".png", p, 
					default_pic_path + experiment_name() + "_Result");
		};
		lattice->simulate(callback, false, experiment_ptr->rich_video);

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

		// save pareto front
		string pareto_path = "results/pareto_" + experiment_name() + "/";
		boost::filesystem::create_directory(pareto_path);
		for (unsigned int i = 0; i < pareto_sol.size(); ++i)
		{
			double fitness(0.0);
			unsigned int index(0);
			ofstream ofile;
			ofile.open(pareto_path + to_string(i + 1) + ".txt");
			for (auto iter = pareto_sol[i].second.begin(); iter != pareto_sol[i].second.end(); ++iter) {
				fitness += *iter;
				ofile << "Objective " << ++index << ": " << *iter << endl;
			}
			ofile << "Aggregate: " << fitness << endl << endl;

			RealVector params = vector_convert(pareto_sol[i].first);
			obj_func->closestFeasible(params);
			real_vector cparams = vector_convert(params);

			ofile << "Params:" << endl;
			ofile.precision(numeric_limits<double>::max_digits10);
			for (auto& param : cparams) ofile << fixed << param << " ";
			ofile.close();

			lattice->get_genotype().get_controller().set_params(cparams);
			simulate(pareto_path, to_string(i + 1) + ".png", false);
		}

		// and save the controller params
		ofstream ofile;
		ofile.open("results/" + experiment_name() + ".txt");
		ofile.precision(numeric_limits<double>::max_digits10);
		// the resulting controller parameters
		//for (auto param : lattice->get_genotype().get_controller().get_params())
		for (auto& param : result_params)
		{
			ofile << fixed << param << " ";
		}
		ofile << endl << endl;
		ofile.close();

		logger::get_logger().log_info("Results for " + experiment_name() + " have been saved!");
	}
}