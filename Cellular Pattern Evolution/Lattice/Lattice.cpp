#include "../Lattice/Headers/Lattice.h"
#include <iostream>

namespace lattice
{
	lattice::lattice(unique_ptr<lattice_settings> ls)
	{
		_settings = move(ls);
		phenotype = phenotypes::create_phenotype(get_settings());
		genotype = unique_ptr<genotype::genotype>(new genotype::genotype(get_settings()));
	}

	void lattice::simulate(lattice_settings ls)
	{

	}
}