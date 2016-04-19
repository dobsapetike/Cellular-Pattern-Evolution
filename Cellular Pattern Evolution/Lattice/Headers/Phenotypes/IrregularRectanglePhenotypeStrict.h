#ifndef IRREGULAR_RECT_PHENOTYPE_STRICT_H
#define IRREGULAR_RECT_PHENOTYPE_STRICT_H

#include "IrregularRectanglePhenotype.h"

namespace lattice
{
	namespace phenotypes
	{
		class irregular_rectangle_cell;	// forward declare

		/**
			Phenotype for an irregular CA containing retangular cells - with strict merge rule
		*/
		class irregular_rectangle_phenotype_strict : public irregular_rectangle_phenotype
		{
		private:
			bool try_surround_merge(shared_ptr<irregular_rectangle_cell> cell);
			bool try_neighbour_merge(shared_ptr<irregular_rectangle_cell> cell, bool right);
		protected:
			virtual bool merge(shared_ptr<irregular_rectangle_cell> cell) override;
		public:
			irregular_rectangle_phenotype_strict(lattice_settings const& settings);
		};
	}
}

#endif