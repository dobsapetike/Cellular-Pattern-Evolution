#include <boost/program_options.hpp>

#include "../Task/Headers/Task.h"
#include "../Task/Headers/ExperimentCollection.h"


using namespace std;
using namespace boost::program_options;

void start_regime(string regime)
{
	/*ofstream ofile;
	ofile.open("results/poly.html");
	ofile << "<svg transform=\"rotate(180, 64, 64)\">" << endl;
	for (auto& r : regions) 
	{
		ofile << "<polygon points=\"" ;
		for (auto& p : r->polygon.outer())
		{
			ofile << p.x() << " " << p.y() << ", ";
		}
		ofile << "\"/>" << endl;
		ofile << "<ellipse cx=\"" << r->generator.get<0>() << "\" cy=\"" << r->generator.get<1>()
			<< "\" rx=\"1\" ry=\"1\" fill=\"red\"/>" << endl;
	}
	ofile << "</svg>" << endl;
	ofile.close();*/
}

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
			t->execute();
			t->simulate();
			t->finalize();
		}
	}

	return EXIT_SUCCESS;
}
