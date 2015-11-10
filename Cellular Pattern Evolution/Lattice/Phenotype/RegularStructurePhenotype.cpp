#include <algorithm>
#include "../Headers/Phenotypes/RegularStructurePhenotype.h"
#include "../Headers/Phenotypes/Cell/RegularCell.h"

namespace lattice
{
	namespace phenotypes
	{
		regular_structure_phenotype::regular_structure_phenotype(lattice_settings const& settings)
			: phenotype(settings)
		{
			shared_ptr<regular_structure_phenotype> selfPtr(this);
			
			// fill the grid with pointers to cells
			_grid.reserve(settings.height);

			for (unsigned int i = 0; i < settings.height; ++i)
			{
				vector<shared_ptr<lattice_cell>> row;
				row.reserve(settings.width);
				for (unsigned int j = 0; j < settings.width; ++j)
				{
					auto cell = shared_ptr<lattice_cell>(
						new regular_cell(j, i, settings.stateSettings, selfPtr));
					row.push_back(cell);
					_cells.push_back(cell);
				}
				_grid.push_back(row);
			}

			// initialize states
			set_init_pattern(settings.init_pattern, settings.stateSettings);

			// set neighbourhood type
			_neighbourhood_type = von_neumann; // default
			if (TiXmlElement* elem = settings.phenotype_settings.get()) 
			{
				auto nbTypeElem = elem->FirstChildElement("NeighbourhoodType");
				if (nbTypeElem) 
				{
					_neighbourhood_type = parse_neighbourhood_type(nbTypeElem->GetText());
				}
			}
		}

		void regular_structure_phenotype::set_init_pattern(string init_pattern, state_settings state_set)
		{
			for (unsigned int y = 0; y < _grid.size(); ++y)
			{
				for (unsigned int x = 0; x < _grid[0].size(); ++x)
				{
					state s = init_state(state_set);
					_grid[y][x].get()->set_state(s);
				}
			}
		}

		neighbourhood regular_structure_phenotype::get_neighbours(lattice_cell const& c) const
		{
			size_t x(c.get_x()), y(c.get_y()), nt(_neighbourhood_type);

			neighbourhood n;
			// upper
			if (y > 0)
			{
				if (x > 0 && nt == moore)
				{
					n[upper].push_back(_grid[y - 1][x - 1]);
					n[direction::left].push_back(_grid[y - 1][x - 1]);
				}
				n[upper].push_back(_grid[y - 1][x]);
				if (x < _grid[0].size() - 1 && nt == moore)
				{
					n[upper].push_back(_grid[y - 1][x + 1]);
					n[direction::right].push_back(_grid[y - 1][x + 1]);
				}
			}

			// middle
			if (x > 0) n[direction::left].push_back(_grid[y][x - 1]);
			if (x < _grid[0].size() - 1) n[direction::right].push_back(_grid[y][x + 1]);

			// bottom
			if (y < _grid.size() - 1)
			{
				if (x > 0 && nt == moore)
				{
					n[direction::left].push_back(_grid[y + 1][x - 1]);
					n[lower].push_back(_grid[y + 1][x - 1]);
				}
				n[lower].push_back(_grid[y + 1][x]);
				if (x < _grid[0].size() - 1 && nt == moore)
				{
					n[direction::right].push_back(_grid[y + 1][x + 1]);
					n[lower].push_back(_grid[y + 1][x + 1]);
				}
			}

			return n;
		}

		neighbourhood_type parse_neighbourhood_type(string type)
		{
			transform(type.begin(), type.end(), type.begin(), ::tolower);
			if (type == "moore")				return moore;
			if (type == "vonneumann")			return von_neumann;

			throw invalid_argument("Unknown neighborhood type");
		}
	}
}