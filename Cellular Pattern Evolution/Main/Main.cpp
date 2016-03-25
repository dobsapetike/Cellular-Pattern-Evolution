#include <boost/program_options.hpp>

#include "../Task/Headers/Task.h"
#include "../Task/Headers/ExperimentCollection.h"

using namespace std;

int main(int argc, char* argv[])
{
	string config = argc > 1 ? argv[1] : "config/confExperiment.xml";

	task::experiments experiments(config);
	for (unsigned int i = 0; i < experiments.experiment_count(); ++i)
	{
		auto& exp = *experiments.at(i);
		unsigned int run(0);
		while (run++ != exp.run_count)
		{
			shared_ptr<task::task> t = make_shared<task::task>(exp, run);
			if (!exp.simulate_only) 
				t->execute();
			t->simulate();
			t->finalize();
		}
	}

	return EXIT_SUCCESS;
}
