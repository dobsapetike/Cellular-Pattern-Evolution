#ifndef PAINTER_H
#define PAINTER_H

#define BPP 24
#define DESIRED_WIDTH 500
#define DESIRED_HEIGHT 500
#define SCALEFACTOR 10

#include "../../Lattice/Headers/Phenotypes/Phenotype.h"

struct FIBITMAP;
struct tagRGBQUAD;

class painter
{
private:
	tagRGBQUAD* border_color;
	const std::string path = "pics/";

	void rasterize_polygon_border(FIBITMAP* bitmap, polygon& p);
	void painter::rasterize_line(FIBITMAP* bitmap,
		boost::geometry::model::d2::point_xy<double> p1, boost::geometry::model::d2::point_xy<double> p2);
public:
	painter();
	~painter();
	void paint(std::string exp, std::string file, lattice::phenotypes::phenotype const& phenotype);
};

#endif