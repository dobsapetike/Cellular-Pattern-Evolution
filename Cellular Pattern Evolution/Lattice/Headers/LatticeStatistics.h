#ifndef LATTICESTATISTICS_H
#define LATTICESTATISTICS_H

namespace lattice
{
	class lattice_statistics
	{
	public:
		unsigned int sim_eval_count = 0;
		unsigned int eval_count = 0;
		real_vector eval_history, gen_history;

		void reset() { sim_eval_count = 0; }
		void add_eval(double fitn)
		{
			if (!eval_history.size()) eval_history.push_back(fitn);
			else eval_history.push_back( min(fitn, eval_history.back()) );
		}
	};
}

#endif