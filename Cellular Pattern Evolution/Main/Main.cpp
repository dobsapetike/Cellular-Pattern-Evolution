#include <boost/program_options.hpp>

#include "Headers/Logger.h"
#include "../Task/Headers/Task.h"
#include "../Task/Headers/ExperimentCollection.h"

#include <map>

using namespace std;
using namespace boost::program_options;

void start_regime(string regime)
{
	task::experiments e("config/confExperiment.xml");
	unique_ptr<task::task> t = make_unique<task::task>(*e[1]);
	//t->simulate();
	t->execute();
	t->finalize();
}

int main(int argc, char* argv[])
{
	try
	{
		options_description description{ "Options" };
		description.add_options()
			("help,h", "Help screen")
			("regime,r", value<string>()->default_value("master"), 
				"Available regimes: master, slave, simulator");
		
		variables_map vm;
		store(parse_command_line(argc, argv, description), vm);
		notify(vm);

		if (vm.count("help"))
		{
			cout << description << endl;
			return EXIT_SUCCESS;
		}

		string regime("master");
		if (vm.count("regime"))
		{
			regime = vm["regime"].as<string>();
		}

		start_regime(regime);
	}
	catch (const error &ex)
	{	
		logger::get_logger().log_error(ex.what());
	}

	logger::get_logger().log_info("DONE! Aplication closes.");
	
	return EXIT_SUCCESS;
}
