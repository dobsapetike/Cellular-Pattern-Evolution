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
	FIBITMAP *bitmap = FreeImage_Allocate(
		phenotype.get_width() * SCALEFACTOR, phenotype.get_height() * SCALEFACTOR, BPP);
	RGBQUAD color;

	auto& cells = phenotype.expose_cells();
	for (auto& cell : cells)
	{
		lattice::rgb cellColor;
		polygon cellPoly = cell->get_geometry(), scaled;
		boost::geometry::strategy::transform::scale_transformer<double, 2, 2> scale(static_cast<double>(SCALEFACTOR));
		boost::geometry::transform(cellPoly, scaled, scale);

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
				if (!boost::geometry::within(p, scaled)) {
					cellColor = { 100, 100, 100 };
				}
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

