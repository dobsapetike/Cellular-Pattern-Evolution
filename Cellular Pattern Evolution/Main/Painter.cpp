#include "Headers/Painter.h"
#include "../Lattice/Headers/Phenotypes/Cell/Cell.h"
#include <FreeImage/FreeImage.h>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/filesystem.hpp>

painter::painter()
{
	boost::filesystem::create_directory(path);
	FreeImage_Initialise();
}

painter::~painter()
{
	FreeImage_DeInitialise();
}

void painter::paint(std::string exp, std::string file, lattice::phenotypes::phenotype const& phenotype)
{
	FIBITMAP *bitmap = FreeImage_Allocate(phenotype.get_width(), phenotype.get_height(), BPP);
	RGBQUAD color;

	auto cells = phenotype.expose_cells();
	for (auto& cell : cells)
	{
		lattice::rgb cellColor = cell->get_state().color;

		polygon cellPoly = cell->get_geometry();
		boost::geometry::model::box<point> box;
		boost::geometry::envelope(cellPoly, box);
		for (unsigned int y = static_cast<unsigned int>(box.min_corner().get<1>());
			y < box.max_corner().get<1>(); ++y)
		{
			for (unsigned int x = static_cast<unsigned int>(box.min_corner().get<0>());
				x < box.max_corner().get<0>(); ++x)
			{
				boost::geometry::model::d2::point_xy<double> p(x, y);
				if (!boost::geometry::covered_by(p, cellPoly)) continue;
				color.rgbRed = cellColor.r;
				color.rgbGreen = cellColor.g;
				color.rgbBlue = cellColor.b;
				FreeImage_SetPixelColor(bitmap, x, y, &color);
			}
		}
	}

	std::string fullPath = path + exp + "/";
	boost::filesystem::create_directory(fullPath);
	FIBITMAP *scaled = FreeImage_Rescale(bitmap, DESIRED_WIDTH, DESIRED_HEIGHT, FILTER_BOX);
	FreeImage_Save(FIF_PNG, scaled, (fullPath + file).c_str(), 0);

	FreeImage_Unload(bitmap);
	FreeImage_Unload(scaled);
}

