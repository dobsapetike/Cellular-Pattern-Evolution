#ifndef OBJFUNC_COLDIST_H
#define OBJFUNC_COLDIST_H

#include "ObjectiveFunction.h"
#include <boost/geometry.hpp>
#include "../../Main/Headers/Logger.h"

namespace objective_functions
{
	/**
		Objective function which tries to minimize the number of cells in the CA
	*/
	class objective_func_color_dist : public objective_func
	{
	private:
		/**
			Needed for normalization
		*/
		unsigned int max_color_diff;
	public:
		objective_func_color_dist(string name, double importance, shared_ptr<lattice::lattice> const& latt)
			: objective_func(name, importance, latt)
		{
			max_color_diff = lattice->get_settings().width * lattice->get_settings().height  
				* 255 * (latt->get_genotype().get_controller().get_color_type() == rgb ? 3 : 1);
		}
		/**
			Evaluation returns the sum of the square of the color difference between
			the target and actual pattern
		*/
		virtual double eval() override
		{
			double sum = 0.0;

			auto& cells = (*this->lattice).get_phenotype().expose_cells();
			auto targets = (*this->lattice).get_settings().target->get_polygons();
			for (auto& cell : cells)
			{
				polygon cellPoly = cell->get_geometry();
				double area = boost::geometry::area(cellPoly), covered(0.0);
				unsigned int coldiffBackground = compute_rgb_distance(cell->get_state().color,
					(*this->lattice).get_settings().target->get_backcolor());

				for (auto& target : targets)
				{
					unsigned int coldiffTarget = compute_rgb_distance(
						cell->get_state().color, target->color);

					vector<polygon> intersection;
					try {
						boost::geometry::intersection(cellPoly, target->polygon, intersection);
					} catch (boost::geometry::overlay_invalid_input_exception exp) {
						logger::get_logger().log_error(exp.what());
						continue;
					}

					for (auto& is : intersection)
					{
						double isArea = boost::geometry::area(is);
						sum += coldiffTarget * isArea;
						covered += isArea;
					}
				}
				sum += (area - covered) * coldiffBackground;
			}

			return sum / static_cast<double>(max_color_diff);
		};
	};
}

#endif