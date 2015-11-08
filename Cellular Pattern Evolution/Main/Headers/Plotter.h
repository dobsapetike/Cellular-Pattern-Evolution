#ifndef PLOTTER_H
#define PLOTTER_H

#include <memory>
#include "gnuplot_i.h"

class plotter
{
private:
	std::vector<double> _history;
	std::unique_ptr<Gnuplot> _canvas;
public:
	plotter() 
	{
		_canvas = std::make_unique<Gnuplot>("linespoints");
		_canvas->showonscreen().set_grid();
	}

	void add_point(double y) 
	{
		_history.push_back(y);
		_canvas->reset_plot();
		_canvas->remove_tmpfiles();
		_canvas->plot_x(_history);
	}
};

#endif