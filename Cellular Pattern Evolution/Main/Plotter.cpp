#include "Headers/Plotter.h"

plotter::plotter()
{
	_canvas = std::make_unique<Gnuplot>("linespoints");
	_canvas->showonscreen().set_grid()
		.set_xlabel("Generation").set_ylabel("Objective");
}

plotter::~plotter()
{
	_canvas->remove_tmpfiles();
}

void plotter::add_point(double y)
{
	_history.push_back(y);
	_canvas->reset_plot();
	_canvas->remove_tmpfiles();
	_canvas->plot_x(_history);
}