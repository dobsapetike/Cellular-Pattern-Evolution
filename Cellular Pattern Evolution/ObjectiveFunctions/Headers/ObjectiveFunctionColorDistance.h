#ifndef OBJFUNC_COLDIST_H
#define OBJFUNC_COLDIST_H

#include "ObjectiveFunction.h"

namespace objective_functions
{
	/**
		Objective function which tries to minimize the number of cells in the CA
	*/
	class objective_func_color_dist : public objective_func
	{
	public:
		objective_func_color_dist(std::string name, std::shared_ptr<lattice::lattice> const& latt)
			: objective_func(name, latt) { }
		/**
			Evaluation returns the sum of the square of the color difference between 
			the target and actual pattern
		*/
		virtual double eval() override {
			double sum = 0.0;

			unsigned int height((*this->lattice).get_settings().height);
			unsigned int width((*this->lattice).get_settings().width);
			for (unsigned int y = 0; y < height; ++y)
			{
				for (unsigned int x = 0; x < width; ++x)
				{
					auto c1 = (*this->lattice).get_settings().target.get()->at(x, y);
					auto c2 = this->lattice.get()->get_phenotype()
						.cell_at(x, y).get()->get_state().color;
					int diff = (c1.r - c2.r) + (c1.g - c2.g) + (c1.b - c2.b);
					sum += abs(diff);
				}
			}

			return sum;
		};
	};
}

#endif