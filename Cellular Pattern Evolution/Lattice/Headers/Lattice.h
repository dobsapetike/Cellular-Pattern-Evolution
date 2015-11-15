#ifndef LATTICE_H
#define LATTICE_H

#include "LatticeSettings.h"
#include "LatticeStatistics.h"
#include "Phenotypes\Phenotype.h"
#include "Genotype\Genotype.h"

using namespace std;

namespace lattice
{
	class lattice final
	{
	private:
		unique_ptr<lattice_statistics> _statistics;
		unique_ptr<lattice_settings> _settings;
		unique_ptr<genotype::genotype> _genotype;
		unique_ptr<phenotypes::phenotype> _phenotype;

		unsigned int _desired_thread_count;
		void update_cells(vector<shared_ptr<phenotypes::lattice_cell>>& cells, 
			unsigned int chunks, unsigned int order_number);
	public:
		explicit lattice(unique_ptr<lattice_settings> ls);
		// getters
		lattice_settings const& get_settings() const { return *_settings; }
		lattice_statistics const& get_statistics() const { return *_statistics; }
		genotype::genotype& get_genotype() const { return *_genotype; }
		phenotypes::phenotype& get_phenotype() const { return *_phenotype; }
		// simulates the computation of the CA
		void simulate(function<void(phenotypes::phenotype&)> = nullptr);
	};
}

#endif