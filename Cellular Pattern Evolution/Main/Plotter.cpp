#include "Headers/Plotter.h"

plotter::plotter()
{
	canvas = std::make_unique<Gnuplot>("linespoints");
	canvas->showonscreen().set_grid()
		.set_xlabel("Generation").set_ylabel("Objective");
}

plotter::~plotter()
{
	canvas->remove_tmpfiles();
}

void plotter::add_point(double y)
{
	history.push_back(y);
	canvas->reset_plot();
	canvas->remove_tmpfiles();
	canvas->plot_x(history);
}