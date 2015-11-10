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
	plotter();
	~plotter();
	void add_point(double y);
};

#endif