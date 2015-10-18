#include <iostream>
#include <memory>
#include <boost/program_options.hpp>

#include "Headers/Task.h"

using namespace std;
using namespace boost::program_options;

void start_regime(string regime)
{
	/* TODO

	if (regime == "master") {
		cout << "TODO start master regime" << endl;
	}
	else if (regime == "slave") {
		cout << "TODO start slave regime" << endl;
	}
	else {
		cout << "TODO start simulation regime" << endl;
	}*/

	task::task t("Config/confTask.xml");
	t.execute();
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
		cerr << ex.what() << endl;
	}

	cout << "DONE" << endl;
	getchar();
	return (EXIT_SUCCESS);
}
