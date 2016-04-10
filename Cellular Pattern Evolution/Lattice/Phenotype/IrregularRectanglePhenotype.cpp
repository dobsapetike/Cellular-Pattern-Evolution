#include "../Headers/Phenotypes/IrregularRectanglePhenotype.h"
#include "../Headers/Phenotypes/Cell/IrregularRectangleCell.h"
#include <numeric> 

namespace lattice
{
	namespace phenotypes
	{
		irregular_rectangle_phenotype::irregular_rectangle_phenotype(lattice_settings const& settings)
			: phenotype(settings)
		{
			self_ptr = shared_ptr<irregular_rectangle_phenotype>(this);
		}

		void irregular_rectangle_phenotype::set_init_pattern(string pattern)
		{
			clear();
			grid.reserve(get_height());

			// fill with grid with unit cells
			for (unsigned int row = 0; row < get_height(); ++row)
			{
				vector<cell_info> rowc;
				rowc.reserve(get_width());
				for (unsigned int col = 0; col < get_width(); ++col)
				{
					// create h x w grid of 1 x 1 cells (default) TODO: read sizes from config
					auto cell = make_shared<irregular_rectangle_cell>(
						col, row, 1, 1, get_state_settings(), self_ptr
					);
					cell->set_state(init_state(get_state_settings()));
					rowc.push_back(cell_info{
						coord{ col, row }, cell
					});
					cells.push_back(cell);
				}
				grid.push_back(rowc);
			}

			// parse and apply pattern
			// apply_pattern(pattern);
		}

		/**
			Understands pattern of form: 'x y width height'
		*/
		void irregular_rectangle_phenotype::apply_pattern(string pattern)
		{
			stringstream sst(pattern);
			unsigned int x, y, w, h;
			while (sst >> x >> y >> w >> h)
			{
				auto cell = make_shared<irregular_rectangle_cell>(
					x, y, w, h, get_state_settings(), self_ptr);
				assign_cell(cell);
			}
		}

		/**
			Empties the cell collections
		*/
		void irregular_rectangle_phenotype::clear()
		{
			cells.clear();
			for (unsigned int i = 0; i < grid.size(); ++i)
				grid[i].clear();
			grid.clear();
		}

		/**
			Inserts given cell into the cell collection
		*/
		void irregular_rectangle_phenotype::assign_cell(shared_ptr<irregular_rectangle_cell> cell)
		{
			// fill the occupied space with nulls
			for (unsigned int row = 0; row < cell->get_height(); ++row)
				for (unsigned int col = 0; col < cell->get_width(); ++col)
				{
					grid[(size_t)(cell->get_y() + row)][(size_t)(cell->get_x() + col)] =
						cell_info{ coord{ cell->get_x(), cell->get_y() }, nullptr };
				}
			cell->set_state(init_state(get_state_settings()));
			// and move the cell
			grid[cell->get_y()][cell->get_x()].cell = move(cell);
		}

		/**
			Splits the rectangle into equally sized (if possible - otherwise almost equally sized) subcells
		*/
		bool irregular_rectangle_phenotype::split(shared_ptr<irregular_rectangle_cell> cell)
		{
			// nothing to do in case of unit cell
			if (cell->get_height() == 1 && cell->get_width() == 1) return false;

			state_settings ss(get_state_settings());
			unsigned int x(cell->get_x()), y(cell->get_y());
			double w(cell->get_width() / 2.0), h(cell->get_height() / 2.0);
			// split cell into four subcells
			vector < shared_ptr<irregular_rectangle_cell> > nc{
				make_shared<irregular_rectangle_cell>(x, y, floor(w), floor(h), ss, self_ptr),
				make_shared<irregular_rectangle_cell>(x + floor(w), y, ceil(w), floor(h), ss, self_ptr),
				make_shared<irregular_rectangle_cell>(x, y + floor(h), floor(w), ceil(h), ss, self_ptr),
				make_shared<irregular_rectangle_cell>(x + floor(w), y + floor(h), ceil(w), ceil(h), ss, self_ptr) 
			};

			for (auto& c : nc)
			{
				// if one of the sides had unit lenght only two subcells are needed
				// so skip the redundant ones - they obviously have side length of 0 
				if (c->get_width() < 1 || c->get_height() < 1) continue;

				c->reset_action();
				assign_cell(c);
			}
			cell.reset();
			return true;
		}

		bool irregular_rectangle_phenotype::merge_side(vector<shared_ptr<lattice_cell>>& neigh,
			shared_ptr<irregular_rectangle_cell> cell, bool right)
		{
			// first, convert the vector to rectangle cells
			vector<shared_ptr<irregular_rectangle_cell>> n(neigh.size());
			for (unsigned int i = 0; i < neigh.size(); ++i)
				n[i] = static_pointer_cast<irregular_rectangle_cell>(neigh[i]);

			// check them, whether they want to merge
			bool ok = true;
			for (auto& c : n)
				ok &= c->get_state().action == action::merge;
			if (!ok) return false;	// nothing to do

			auto size_prop = [right](const shared_ptr<irregular_rectangle_cell> c) {
				return right ? c->get_width() : c->get_height();
			};
			auto dif_prop = [right](const shared_ptr<irregular_rectangle_cell> c) {
				return !right ? c->get_width() : c->get_height();
			};
			//if ((right && cell->get_y() + cell->get_height() != n.back()->get_y() + n.back()->get_height())) return false;
			//if ((!right && cell->get_x() + cell->get_width() != n.back()->get_x() + n.back()->get_width())) return false;

			// compute added size
			auto minElem = *std::min_element(n.begin(), n.end(),
				[size_prop](const shared_ptr<irregular_rectangle_cell> a, const shared_ptr<irregular_rectangle_cell> b) -> bool {
				return size_prop(a) < size_prop(b);
			});
			unsigned int chunksize = size_prop(minElem);

			// create merged cell
			auto merged = make_shared<irregular_rectangle_cell>(
				cell->get_x(), cell->get_y(),
				cell->get_width() + (right ? chunksize : 0), cell->get_height() + (right ? 0 : chunksize),
				get_state_settings(), self_ptr);
			assign_cell(merged);

			// handle border cells 
			// at the beggining
			if ((right && cell->get_y() != n[0]->get_y()) || (!right && cell->get_x() != n[0]->get_x()))
			{
				auto remaining = make_shared<irregular_rectangle_cell>(
					n[0]->get_x(), n[0]->get_y(),
					right ? n[0]->get_width() : cell->get_x() - n[0]->get_x(),
					right ? cell->get_y() - n[0]->get_y() : n[0]->get_height(),
					get_state_settings(), self_ptr);
				n[0] = make_shared<irregular_rectangle_cell>(
					right ? n[0]->get_x() : cell->get_x(),
					right ? cell->get_y() : n[0]->get_y(),
					n[0]->get_width() - (right ? 0 : remaining->get_width()),
					n[0]->get_height() - (right ? remaining->get_height() : 0),
					get_state_settings(), self_ptr);
				assign_cell(remaining);
			}
			// at the end
			if ((right && cell->get_y() + cell->get_height() != n.back()->get_y() + n.back()->get_height())
				|| (!right && cell->get_x() + cell->get_width() != n.back()->get_x() + n.back()->get_width()))
			{
				unsigned pwidth(n.back()->get_width()), pheight(n.back()->get_height());
				n[n.size() - 1] = make_shared<irregular_rectangle_cell>(
					n.back()->get_x(), n.back()->get_y(),
					right ? n.back()->get_width() : cell->get_x() + cell->get_width() - n.back()->get_x(),
					right ? cell->get_y() + cell->get_height() - n.back()->get_y() : n.back()->get_height(),
					get_state_settings(), self_ptr);
				auto remaining = make_shared<irregular_rectangle_cell>(
					right ? n.back()->get_x() : cell->get_x() + cell->get_width(),
					right ? cell->get_y() + cell->get_height() : n.back()->get_y(),
					pwidth - (right ? 0 : n.back()->get_width()),
					pheight - (right ? n.back()->get_height() : 0),
					get_state_settings(), self_ptr);
				assign_cell(remaining);
			}

			// cut chunk from neighbours
			for (auto& nc : n)
			{
				if (size_prop(nc) == chunksize) continue;	// removed entirely
				auto remaining = make_shared<irregular_rectangle_cell>(
					nc->get_x() + (right ? chunksize : 0),
					nc->get_y() + (right ? 0 : chunksize),
					nc->get_width() - (right ? chunksize : 0),
					nc->get_height() - (right ? 0 : chunksize),
					get_state_settings(), self_ptr);
				assign_cell(remaining);
				nc.reset();
			}

			cell.reset();
			return true;
		}

		bool irregular_rectangle_phenotype::merge(shared_ptr<irregular_rectangle_cell> cell)
		{
			unsigned int x(cell->get_x()), y(cell->get_y()), 
				w(cell->get_width()), h(cell->get_height());


			// check right and bottom neighbourhood for merging opportunity
			if (x + w < get_width())
			{
				auto right = get_distinct_cells_in_line(y, y + h, x + w, true);
				if (merge_side(right, cell, true)) return true;
			}
			if (y + h < get_height())
			{
				auto bottom = get_distinct_cells_in_line(x, x + w, y + h, false);
				return merge_side(bottom, cell, false);
			}

			return false;
		}

		merge_split_count irregular_rectangle_phenotype::rearrange_topology()
		{
			// drop the previous cells collection and 
			// fill the automaton with new ones based on their action state
			merge_split_count msc(0, 0);
			cells.clear();
			for (unsigned int row = 0; row < get_height(); ++row)
			{
				for (unsigned int col = 0; col < get_width(); ++col)
				{
					auto& cell = grid[row][col].cell;
					if (cell == nullptr) continue;
					switch (cell->get_state().action)
					{
						case action::merge:
							if (merge(cell)) msc.first++;
							break;
						case action::split:
							if (split(cell)) msc.second++;
							break;
						default:
							break;
					}
					cells.push_back(grid[row][col].cell);
				}
			}
			return msc;
		}

		/**
			Returns a vector of cells occupying	the given line in the CA	
		*/
		vector<shared_ptr<lattice_cell>> irregular_rectangle_phenotype::get_distinct_cells_in_line(
			unsigned int start, unsigned int end,
			unsigned int constdim, bool vertical) const
		{
			vector<shared_ptr<lattice_cell>> cells;
			coord lastCoord{ -1, -1 };
			for (unsigned int i = start; i < end; ++i)
			{
				auto coord = vertical ? grid[i][constdim].cell_coord : grid[constdim][i].cell_coord;
				if (coord.x == lastCoord.x && coord.y == lastCoord.y) continue;
				cells.push_back(grid[coord.y][coord.x].cell);
				lastCoord = coord;
			}
			return cells;
		}

		neighbourhood irregular_rectangle_phenotype::get_neighbours(lattice_cell const& c) const
		{
			auto& cell = static_cast<const irregular_rectangle_cell&>(c);
			neighbourhood n;
			unsigned int x(cell.get_x()), y(cell.get_y()), w(cell.get_width()), h(cell.get_height());
			if (y > 0) n[upper] = get_distinct_cells_in_line(x, x + w, y - 1, false);
			if (y + h < get_height() - 1) n[lower] = get_distinct_cells_in_line(x, x + w, y + h, false);
			if (x > 0) n[direction::left] = get_distinct_cells_in_line(y, y + h, x - 1, true);
			if (x + w < get_width() - 1) n[direction::right] = get_distinct_cells_in_line(y, y + h, x + w, true);
			return n;
		}
	}
}


/*bool irregular_rectangle_phenotype::merge_side(vector<shared_ptr<lattice_cell>>& neigh,
shared_ptr<irregular_rectangle_cell> cell, bool right)
{
// first, convert the vector to rectangle cells
vector<shared_ptr<irregular_rectangle_cell>> n(neigh.size());
for (unsigned int i = 0; i < neigh.size(); ++i)
n[i] = static_pointer_cast<irregular_rectangle_cell>(neigh[i]);

// check them, whether they want to merge
bool ok = true;
for (auto& c : n)
ok &= c->get_state().action == action::merge;
if (!ok) return false;	// nothing to do

auto size_prop = [right](const shared_ptr<irregular_rectangle_cell> c) {
return right ? c->get_width() : c->get_height();
};
auto dif_prop = [right](const shared_ptr<irregular_rectangle_cell> c) {
return !right ? c->get_width() : c->get_height();
};
if (right && cell->get_y() != n[0]->get_y()) return false;
if (!right && cell->get_x() != n[0]->get_x()) return false;
if (dif_prop(cell) != std::accumulate(n.begin(), n.end(), 0, [dif_prop]
(int curr, const shared_ptr<irregular_rectangle_cell> a) { return curr + dif_prop(a); }) ) return false;

// compute added size
auto minElem = *std::min_element(n.begin(), n.end(),
[size_prop](const shared_ptr<irregular_rectangle_cell> a, const shared_ptr<irregular_rectangle_cell> b) -> bool {
return size_prop(a) < size_prop(b);
});
unsigned int chunksize = size_prop(minElem);

// create merged cell
auto merged = make_shared<irregular_rectangle_cell>(
cell->get_x(), cell->get_y(),
cell->get_width() + (right ? chunksize : 0), cell->get_height() + (right ? 0 : chunksize),
get_state_settings(), self_ptr);
assign_cell(merged);

// cut chunk from neighbours
for (auto& nc : n)
{
if (size_prop(nc) == chunksize) continue;	// removed entirely
auto remaining = make_shared<irregular_rectangle_cell>(
nc->get_x() + (right ? chunksize : 0),
nc->get_y() + (right ? 0 : chunksize),
nc->get_width() - (right ? chunksize : 0),
nc->get_height() - (right ? 0 : chunksize),
get_state_settings(), self_ptr);
assign_cell(remaining);
nc.reset();
}

cell.reset();
return true;
} */