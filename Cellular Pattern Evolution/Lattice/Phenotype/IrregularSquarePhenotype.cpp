#include "../Headers/Phenotypes/IrregularSquarePhenotype.h"
#include "../Headers/Phenotypes/Cell/IrregularSquareCell.h"

namespace lattice
{
	namespace phenotypes
	{
		/**
			Understands pattern of form: 'x y dimension'
		*/
		void irregular_square_phenotype::apply_pattern(string pattern)
		{
			stringstream sst(pattern);
			unsigned int x, y, d;
			while (sst >> x >> y >> d)
			{
				auto cell = make_shared<irregular_square_cell>(
					x, y, d, get_state_settings(), cast_self_ptr());
				assign_cell(cell);
			}
		}

		/**
			Splits the cell into four subcells of equal size - in case of non-unit cells
		*/
		bool irregular_square_phenotype::split(shared_ptr<irregular_rectangle_cell> cell)
		{
			// dont split if dimension of square has unit length
			if (cell->get_width() == 1) return false;

			state_settings ss(get_state_settings());
			unsigned int x(cell->get_x()), y(cell->get_y()), w(cell->get_width() / 2);
			// split cell into four subcells (placing the make_shared directly in initializer produced runtime errors, separating)
			shared_ptr<irregular_square_cell> c1 = make_shared<irregular_square_cell>(x, y, w, ss, cast_self_ptr());
			shared_ptr<irregular_square_cell> c2 = make_shared<irregular_square_cell>(x + w, y, w, ss, cast_self_ptr());
			shared_ptr<irregular_square_cell> c3 = make_shared<irregular_square_cell>(x, y + w, w, ss, cast_self_ptr());
			shared_ptr<irregular_square_cell> c4 = make_shared<irregular_square_cell>(x + w, y + w, w, ss, cast_self_ptr());

			vector < shared_ptr<irregular_square_cell> > nc{ c1, c2, c3, c4 };
			for (auto c : nc) c->set_state(cell->get_state());
			cell.reset();
						
			for (auto c : nc) assign_cell(c);
			return true;
		}

		bool irregular_square_phenotype::merge(shared_ptr<irregular_rectangle_cell> cell)
		{
			// skip cells on border
			if (cell->get_x() + cell->get_width() >= get_width() ||
				cell->get_y() + cell->get_height() >= get_height()) return false;

			// only merge if result is also square
			vector < shared_ptr<irregular_rectangle_cell> > mNeigh{
				grid[cell->get_y()][cell->get_x() + cell->get_width()].cell,
				grid[cell->get_y() + cell->get_height()][cell->get_x()].cell,
				grid[cell->get_y() + cell->get_height()][cell->get_x() + cell->get_width()].cell };
			for (auto& n : mNeigh)
			{
				if (n == nullptr) return false;
				if (n->get_height() != cell->get_height()) return false;
				if (n->get_state().action != action::merge) return false;
			}

			// everything clicks, so go for it
			auto merged = make_shared<irregular_square_cell>(
				cell->get_x(), cell->get_y(), cell->get_width() * 2, get_state_settings(), cast_self_ptr());
			merged->set_state(merge_state(mNeigh));
			cell.reset();
			assign_cell(merged);
			return true;
		}
	}
}