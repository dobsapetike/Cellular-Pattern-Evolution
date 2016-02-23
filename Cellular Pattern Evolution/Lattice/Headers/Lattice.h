#ifndef LATTICE_H
#define LATTICE_H

#include "LatticeSettings.h"
#include "LatticeStatistics.h"
#include "Phenotypes/Phenotype.h"
#include "Genotype/Genotype.h"
#include "../../Main/Headers/ThreadPool.h"

using namespace std;

namespace lattice
{
	class lattice final
	{
	private:
		unique_ptr<lattice_statistics> statistics;
		unique_ptr<lattice_settings> settings;
		unique_ptr<genotype::genotype> genotype;
		unique_ptr<phenotypes::phenotype> phenotype;

		unique_ptr<thread_pool> threads;
		void update_cells(unsigned int order_number);
	public:
		lattice(unique_ptr<lattice_settings> ls);
		// getters
		lattice_settings const& get_settings() const { return *settings; }
		lattice_statistics& get_statistics() const { return *statistics; }
		genotype::genotype& get_genotype() const { return *genotype; }
		phenotypes::phenotype& get_phenotype() const { return *phenotype; }
		// simulates the computation of the CA
		void simulate(function<void(phenotypes::phenotype&)> = nullptr);
	};
}

#endif