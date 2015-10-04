#ifndef LATTICE_H
#define LATTICE_H

#include "LatticeSettings.h"
#include "Phenotypes\Phenotype.h"
#include "Genotype\Genotype.h"

using namespace std;

namespace lattice
{
	class lattice
	{
	private:
		unique_ptr<lattice_settings> _settings;
	protected:
		unique_ptr<genotype::genotype> genotype;
		unique_ptr<phenotypes::phenotype> phenotype;
	public:
		lattice(unique_ptr<lattice_settings> ls);
		lattice_settings const& get_settings() const { return *_settings; }
		// static method, which gets the settings of a lattice and an 
		// evolved controller (TODO), and simulates the computation of 
		// the CA, returning the resulting svg (TODO)
		static void simulate(lattice_settings ls);
	};
}

#endif