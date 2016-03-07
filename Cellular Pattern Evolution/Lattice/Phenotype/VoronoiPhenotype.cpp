#include "../Headers/Phenotypes/VoronoiPhenotye.h"
#include "../Headers/Phenotypes/Cell/VoronoiCell.h"

namespace lattice
{
	namespace phenotypes
	{
		voronoi_phenotype::voronoi_phenotype(lattice_settings const& settings)
			: phenotype(settings)
		{
			self_ptr = shared_ptr<voronoi_phenotype>(this);
		}

		void voronoi_phenotype::place_incident(v_edge const& edge, shared_ptr<voronoi_cell> cell)
		{
			auto flipped = edge.flipped();
			if (incidents.count(edge)) incidents[edge].push_back(cell);
			else if (incidents.count(flipped)) incidents[flipped].push_back(cell);
			else incidents[edge].push_back(cell);
		}

		void voronoi_phenotype::generate_topology(vector<point> const& points,
			unordered_map<point, state, point_hash>* state_map)
		{
			cells.clear();
			incidents.clear();

			auto diagram = vor_factory.create_diagram(points, get_width(), get_height());
			for (auto region : diagram)
			{
				auto cellptr = make_shared<voronoi_cell>(region, get_state_settings(), self_ptr);
				if (state_map) {
					auto stateitr = state_map->find(cellptr->get_region()->generator);
					cellptr->set_state(stateitr->second);
				}

				cells.push_back(cellptr);
				auto& vertices = region->polygon.outer();
				for (unsigned int i = 0; i < vertices.size() - 1; ++i) {
					v_edge edge = v_edge{ vertices[i].x(), vertices[i].y(),
						vertices[i + 1].x(), vertices[i + 1].y(), true };
					place_incident(edge, cellptr);
				}
			}
		}

		void voronoi_phenotype::split(shared_ptr<voronoi_cell> cell, vector<point>& new_points)
		{
			double minx(FLT_MAX), miny(FLT_MAX), maxx(FLT_MIN), maxy(FLT_MIN);
			auto& points = cell->get_region()->polygon.outer();
			for (auto& p : points)
			{
				minx = min(minx, p.x()); miny = min(miny, p.y());
				maxx = max(maxx, p.x()); maxy = max(maxy, p.y());
			}
			auto& gen = cell->get_region()->generator;
			double gx(gen.get<0>()), gy(gen.get<1>());

			new_points.clear();
			if (gx - minx > 3.0) new_points.push_back(point(gx - (gx - minx) / 2.0, gy));
			if (maxx - gx > 3.0) new_points.push_back(point(maxx - (maxx - gx) / 2.0, gy));
			if (gy - miny > 3.0) new_points.push_back(point(gx, gy - (gy - miny) / 2.0));
			if (maxy - gy > 3.0) new_points.push_back(point(gx, maxy - (maxy - gy) / 2.0));
		}

		bool voronoi_phenotype::merge(shared_ptr<voronoi_cell> cell, point& point)
		{
			auto n = get_neighbours(*cell);
			shared_ptr<voronoi_cell> mergcell = nullptr;
			for (unsigned int d = upper; d <= lower; ++d) {
				direction dir = static_cast<direction>(d);
				if (!n[dir].size()) continue;
				for (auto c : n[dir])  
				{
					if (c->get_state().action == action::merge) {
						mergcell = static_pointer_cast<voronoi_cell>(c);
						break;
					}
				}
				if (mergcell) break;
			}

			if (!mergcell) return false;
			auto& a = mergcell->get_region()->generator;
			auto& b = cell->get_region()->generator;
			point = ::point((a.get<0>() + b.get<0>()) / 2.0, (a.get<1>() + b.get<1>()) / 2.0);
			mergcell->reset_action(nil);
			return true;
		}

		void voronoi_phenotype::rearrange_topology()
		{
			point np;
			vector<point> new_points, nps;
			unordered_map<point, state, point_hash> state_map;
			unsigned int added;

			for (auto cell : cells)
			{
				auto vcell = static_pointer_cast<voronoi_cell>(cell);
				switch (vcell->get_state().action)
				{
					/*case action::merge:
						if (merge(vcell, np))
							new_points.push_back(np);
						break;*/
					case action::split:
						split(vcell, nps);
						added = 0;
						for (auto& p : nps) {
							if (!boost::geometry::within(p, vcell->get_region()->polygon)) continue;
							new_points.push_back(p);
							state_map[p] = vcell->get_state();
							++added;
						}
						if (!added) {
							new_points.push_back(vcell->get_region()->generator);
							state_map[vcell->get_region()->generator] = vcell->get_state();
						}
						break;
					case action::nil:
						break;
					default:
						new_points.push_back(vcell->get_region()->generator);
						state_map[vcell->get_region()->generator] = vcell->get_state();
						break;
				}
			}
			generate_topology(new_points, &state_map);
			//generate_topology(new_points);
		}

		void voronoi_phenotype::set_init_pattern(string pattern)
		{
			stringstream sst(pattern);
			unsigned int x, y;
			vector<point> new_points;
			while (sst >> x >> y) {
				new_points.push_back(point(x, y));
			}
			generate_topology(new_points);
		}

		void voronoi_phenotype::get_edge_directions(v_edge const& edge, bool* directions) const
		{
			directions[::left] = edge.y2 > edge.y1 && !float_equal(edge.y1, edge.y2);
			directions[::right] = edge.y1 > edge.y2 && !float_equal(edge.y1, edge.y2);
			directions[upper] = edge.x2 > edge.x1 && !float_equal(edge.x1, edge.x2);
			directions[lower] = edge.x1 > edge.x2 && !float_equal(edge.x1, edge.x2);
		}

		neighbourhood voronoi_phenotype::get_neighbours(lattice_cell const& c) const
		{
			neighbourhood n;
			auto& cell = static_cast<const voronoi_cell&>(c);
			auto& points = cell.get_region()->polygon.outer();

			bool dirs[4];
			for (unsigned int i = 0; i < points.size() - 1; ++i)
			{
				v_edge edge = v_edge{ points[i].x(), points[i].y(), 
					points[i + 1].x(), points[i + 1].y(), true };
				 auto& inc = incidents.find(edge)->second;
				 if (inc.size() == 1) continue; // side edge
				 auto nptr = point_equal(inc[0]->get_region()->generator, 
					 cell.get_region()->generator) ? inc[1] : inc[0];

				 get_edge_directions(edge, dirs);
				 for (unsigned int dir = upper; dir <= lower; ++dir) {
					 if (dirs[dir]) n[static_cast<direction>(dir)].push_back(nptr);
				 }
			}
			return n;
		}
	}
}