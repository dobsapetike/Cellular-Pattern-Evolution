#include "../Headers/Phenotypes/IrregularRectanglePhenotypeStrict.h"
#include "../Headers/Phenotypes/Cell/IrregularRectangleCell.h"

namespace lattice
{
	namespace phenotypes
	{
		irregular_rectangle_phenotype_strict::irregular_rectangle_phenotype_strict(lattice_settings const& settings)
			: irregular_rectangle_phenotype(settings)
		{
		}

		bool irregular_rectangle_phenotype_strict::try_surround_merge(shared_ptr<irregular_rectangle_cell> cell)
		{
			vector < shared_ptr<irregular_rectangle_cell> > mNeigh{
				grid[cell->get_y()][cell->get_x() + cell->get_width()].cell,
				grid[cell->get_y() + cell->get_height()][cell->get_x()].cell,
				grid[cell->get_y() + cell->get_height()][cell->get_x() + cell->get_width()].cell };
			for (auto& n : mNeigh)
			{
				if (n == nullptr) return false;
				if (n->get_width() != cell->get_width()) return false;
				if (n->get_height() != cell->get_height()) return false;
				if (n->get_state().action != action::merge) return false;
			}

			auto merged = make_shared<irregular_rectangle_cell>(
				cell->get_x(), cell->get_y(), 2 * cell->get_width(), 
				2 * cell->get_height(), get_state_settings(), self_ptr
			);
			merged->set_state(merge_state(mNeigh));
			cell.reset();
			assign_cell(merged);
			return true;
		}

		bool irregular_rectangle_phenotype_strict::try_neighbour_merge(shared_ptr<irregular_rectangle_cell> cell, bool right)
		{
			auto neigh =  right ? grid[cell->get_y()][cell->get_x() + cell->get_width()].cell
				: grid[cell->get_y() + cell->get_height()][cell->get_x()].cell;
			if (neigh == nullptr) return false;
			if (neigh->get_state().action != action::merge) return false;
			if (right && neigh->get_height() != cell->get_height()) return false;
			if (!right && neigh->get_width() != cell->get_width()) return false;

			auto merged = make_shared<irregular_rectangle_cell>(
				cell->get_x(), cell->get_y(), 
				right ? cell->get_width() + neigh->get_width() : cell->get_width(),
				right ? cell->get_height() : cell->get_height() + neigh->get_height(),
				get_state_settings(), self_ptr
			);
			auto mv = vector < shared_ptr<irregular_rectangle_cell> > { cell, merged };
			merged->set_state(merge_state(mv));
			cell.reset();
			assign_cell(merged);
			return true;
		}

		bool irregular_rectangle_phenotype_strict::merge(shared_ptr<irregular_rectangle_cell> cell)
		{
			if (cell->get_x() + cell->get_width() >= get_width() ||
				cell->get_y() + cell->get_height() >= get_height()) return false;

			// try merge with the surrounding if the dimensionality matches
			if (try_surround_merge(cell)) return true;
			// else try in one direction
			if (try_neighbour_merge(cell, true)) return true;
			if (try_neighbour_merge(cell, false)) return true;

			return false;
		}
	}
}