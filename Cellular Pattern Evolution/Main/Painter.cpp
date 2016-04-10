#include "Headers/Painter.h"
#include "../Lattice/Headers/Phenotypes/Cell/Cell.h"
#include <FreeImage/FreeImage.h>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/filesystem.hpp>

painter::painter()
{
	boost::filesystem::create_directory(default_pic_path);
	FreeImage_Initialise();

	border_color = new RGBQUAD;
	border_color->rgbBlue = border_color->rgbGreen = border_color->rgbRed = 100;
}

painter::~painter()
{
	delete border_color;
	FreeImage_DeInitialise();
}

void painter::rasterize_line(FIBITMAP* bitmap,
	boost::geometry::model::d2::point_xy<double> p1, boost::geometry::model::d2::point_xy<double> p2)
{
	int sx(round(p1.x())), sy(round(p1.y())), ex(round(p2.x())), ey(round(p2.y()));
	int dx(ex - sx), dy(ey - sy);
	FreeImage_SetPixelColor(bitmap, sx, sy, border_color);
	if (abs(dx) >= abs(dy))
	{
		if (dx == 0) return;
		int x = sx, xinc = dx > 0 ? 1 : -1;
		double y = sy, yinc = static_cast<double>(dy) / dx * xinc;
		while (x != ex)
		{
			x += xinc;
			y += yinc;
			FreeImage_SetPixelColor(bitmap, x, round(y), border_color);
		}
	}
	else
	{
		if (dy == 0) return;
		int y = sy, yinc = dy > 0 ? 1 : -1;
		double x = sx, xinc = static_cast<double>(dx) / dy * yinc;
		while (y != ey)
		{
			x += xinc;
			y += yinc;
			FreeImage_SetPixelColor(bitmap, round(x), y, border_color);
		}
	}
}

void painter::rasterize_polygon_border(FIBITMAP* bitmap, polygon& p)
{
	for (unsigned int i = 0; i < p.outer().size() - 1; ++i)
		rasterize_line(bitmap, p.outer()[i], p.outer()[i + 1]);
	rasterize_line(bitmap, p.outer()[0], p.outer()[p.outer().size() - 1]);
}

void painter::paint(string file, lattice::phenotypes::phenotype const& phenotype, string path)
{
	FIBITMAP *bitmap = FreeImage_Allocate(
		phenotype.get_width() * SCALEFACTOR, phenotype.get_height() * SCALEFACTOR, BPP);
	RGBQUAD color;

	auto& cells = phenotype.expose_cells();
	vector<polygon> sc_polys;
	for (auto& cell : cells)
	{
		lattice::rgb cellColor;
		polygon cellPoly = cell->get_geometry(), scaled;
		boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scale(static_cast<double>(SCALEFACTOR));
		boost::geometry::transform(cellPoly, scaled, scale);
		sc_polys.push_back(scaled);

		boost::geometry::model::box<point> box;
		boost::geometry::envelope(scaled, box);
		for (unsigned int y = static_cast<unsigned int>(box.min_corner().get<1>());
			y <= box.max_corner().get<1>(); ++y)
		{
			for (unsigned int x = static_cast<unsigned int>(box.min_corner().get<0>());
				x <= box.max_corner().get<0>(); ++x)
			{
				boost::geometry::model::d2::point_xy<double> p(x, y);
				if (!boost::geometry::covered_by(p, scaled)) continue;
				cellColor = cell->get_state().color;

				color.rgbRed = cellColor.r;
				color.rgbGreen = cellColor.g;
				color.rgbBlue = cellColor.b;
				FreeImage_SetPixelColor(bitmap, x, y, &color);
			}
		}
	}

	for (auto& poly : sc_polys) rasterize_polygon_border(bitmap, poly);

	string fullPath = path + "/";
	boost::filesystem::create_directory(fullPath);
	FIBITMAP *scaled = FreeImage_Rescale(bitmap, DESIRED_WIDTH, DESIRED_HEIGHT, FILTER_BOX);
	FreeImage_Save(FIF_PNG, scaled, (fullPath + file).c_str(), 0);

	FreeImage_Unload(bitmap);
	FreeImage_Unload(scaled);
}

