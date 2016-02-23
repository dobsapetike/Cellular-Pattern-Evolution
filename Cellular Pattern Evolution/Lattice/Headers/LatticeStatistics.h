#ifndef LATTICESTATISTICS_H
#define LATTICESTATISTICS_H

namespace lattice
{
	class lattice_statistics
	{
	public:
		unsigned int sim_eval_count = 0;
		unsigned int eval_count = 0;
		void reset() { sim_eval_count = 0; }
	};
}

#endif