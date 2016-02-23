#include "../Headers/Phenotypes/IrregularSquarePhenotype.h"
#include "../Headers/Phenotypes/Cell/IrregularSquareCell.h"

namespace lattice
{
	namespace phenotypes
	{
		void irregular_square_phenotype::split(shared_ptr<irregular_rectangle_cell> cell)
		{
			// dont split if dimension of square has unit length
			if (cell->get_width() == 1) return;

			state_settings ss(get_state_settings());
			unsigned int x(cell->get_x()), y(cell->get_y()), w(cell->get_width() / 2);
			// split cell into four subcells
			vector < shared_ptr<irregular_square_cell> > nc {
				make_shared<irregular_square_cell>(x, y, w, ss, cast_self_ptr()),
				make_shared<irregular_square_cell>(x + w, y, w, ss, cast_self_ptr()),
				make_shared<irregular_square_cell>(x, y + w, w, ss, cast_self_ptr()),
				make_shared<irregular_square_cell>(x + w, y + w, w, ss, cast_self_ptr()) };
			cell.reset();
			for (auto& c : nc) 
				assign_cell(c);
		}

		void irregular_square_phenotype::merge(shared_ptr<irregular_rectangle_cell> cell)
		{
			// skip cells on border
			if (cell->get_x() + cell->get_width() >= get_width() ||
				cell->get_y() + cell->get_height() >= get_height()) return;

			// only merge if result is also square
			vector < shared_ptr<irregular_rectangle_cell> > mNeigh{
				grid[cell->get_y()][cell->get_x() + cell->get_width()].cell,
				grid[cell->get_y() + cell->get_height()][cell->get_x()].cell,
				grid[cell->get_y() + cell->get_height()][cell->get_x() + cell->get_width()].cell };
			for (auto& n : mNeigh)
			{
				if (n == nullptr) return;
				if (n->get_height() != cell->get_height()) return;
				if (n->get_state().action != action::merge) return;
			}

			// everything clicks, so go for it
			auto merged = make_shared<irregular_square_cell>(
				cell->get_x(), cell->get_y(), cell->get_width() * 2, get_state_settings(), cast_self_ptr());
			cell.reset();
			assign_cell(merged);
		}
	}
}