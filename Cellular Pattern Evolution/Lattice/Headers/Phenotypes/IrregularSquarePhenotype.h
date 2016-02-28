#ifndef IRREGULAR_SQUARE_PHENOTYPE_H
#define IRREGULAR_SQUARE_PHENOTYPE_H

#include "IrregularRectanglePhenotype.h"

namespace lattice
{
	namespace phenotypes
	{
		class irregular_square_cell;	// forward declare

		/**
			Phenotype for an irregular CA containing square cells
		*/
		class irregular_square_phenotype : public irregular_rectangle_phenotype
		{
		private:
			shared_ptr<irregular_square_phenotype> cast_self_ptr() const {
				return static_pointer_cast<irregular_square_phenotype>(self_ptr);
			}
		protected:
			void apply_pattern(string pattern) override;
			void split(shared_ptr<irregular_rectangle_cell> cell) override;
			void merge(shared_ptr<irregular_rectangle_cell> cell) override;
		public:
			irregular_square_phenotype(lattice_settings const& settings) 
				: irregular_rectangle_phenotype(settings) { };
			cell_type get_cell_type() const override { return irregular_square; };
		};
	}
}

#endif