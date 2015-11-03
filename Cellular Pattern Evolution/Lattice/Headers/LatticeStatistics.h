#ifndef LATTICESTATISTICS_H
#define LATTICESTATISTICS_H

namespace lattice
{
	class lattice_statistics
	{
	public:
		unsigned int eval_count = 0;
		void reset() { eval_count = 0; }
	};
}

#endif