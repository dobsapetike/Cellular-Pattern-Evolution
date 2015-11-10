#ifndef PAINTER_H
#define PAINTER_H

#define BPP 24
#define DESIRED_WIDTH 500
#define DESIRED_HEIGHT 500

#include "../../Lattice/Headers/Phenotypes/Phenotype.h"

class painter
{
private:
	const std::string path = "pics/";
public:
	painter();
	~painter();
	void paint(std::string exp, std::string file, lattice::phenotypes::phenotype const& phenotype);
};

#endif