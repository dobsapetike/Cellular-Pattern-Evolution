#ifndef LATTICE_H
#define LATTICE_H

#include "LatticeSettings.h"
#include "LatticeStatistics.h"
#include "Phenotypes/Phenotype.h"
#include "Genotype/Genotype.h"
#include "../../Main/Headers/ThreadPool.h"

using namespace std;

namespace objective_functions {
	class objective_func_color_dist;
}

namespace lattice
{
	class lattice : public enable_shared_from_this<lattice>
	{
	private:
		unique_ptr<lattice_statistics> statistics;
		unique_ptr<lattice_settings> settings;
		unique_ptr<genotype::genotype> genotype;
		unique_ptr<phenotypes::phenotype> phenotype;

		unique_ptr<thread_pool> threads;

		void init_evaluator();
		unique_ptr<objective_functions::objective_func_color_dist> evaluator;

		void update_cells(unsigned int order_number);
	public:
		lattice(unique_ptr<lattice_settings> ls);
		~lattice();
		// getters
		lattice_settings const& get_settings() const { return *settings; }
		lattice_statistics& get_statistics() const { return *statistics; }
		genotype::genotype& get_genotype() const { return *genotype; }
		phenotypes::phenotype& get_phenotype() const { return *phenotype; }
		// simulates the computation of the CA
		void simulate(function<void(phenotypes::phenotype&)> = nullptr, 
			bool observed_run = false, bool frequentcallback = false);
	};
}

#endif