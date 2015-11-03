#include "Headers/TargetPatternSvg.h"
#include <tinyxml/tinyxml.h>

namespace lattice
{
	target_pattern_svg::target_pattern_svg(
		unsigned int desired_width, unsigned int desired_height, string filepath)
	{
		TiXmlDocument target(filepath.c_str());
		if (!target.LoadFile())
			throw invalid_argument("Could not parse file: " + filepath);

		auto svgElem = target.FirstChildElement("svg");
		int width(atoi(svgElem->Attribute("width")));
		int height(atoi(svgElem->Attribute("height")));
		if (width != desired_width || height != desired_height)
			throw invalid_argument("Mismatch between target pattern size and CA size!");

		auto polyElem = svgElem->FirstChildElement();
		while (polyElem)
		{
			if (strcmpi(polyElem->Value(), "polygon"))
				throw invalid_argument("Target pattern has to be specified using \"polygon\"!");

			auto p = make_shared<color_polygon>();
			p->color = parse_rgb(polyElem->Attribute("fill"));

			string points = polyElem->Attribute("points");
			// convert svg format to wkt
			replace(points.begin(), points.end(), ',', '$');
			replace(points.begin(), points.end(), ' ', ',');
			replace(points.begin(), points.end(), '$', ' ');
			boost::geometry::read_wkt("POLYGON((" + points + "))", p->polygon);
			_polygons.push_back(move(p));

			polyElem = polyElem->NextSiblingElement();
		}
	}
}