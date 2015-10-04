#ifndef FEED_FORWARD_ANN_CONTROLLER_H
#define FEED_FORWARD_ANN_CONTROLLER_H

#include <armadillo>
#include "StateController.h"

namespace lattice
{
	namespace controllers
	{
		const int BIAS = -1;

		/**
			Controller using a multilayer perceptron as the base model of computation
		*/
		class feedforward_ann_controller : public state_controller
		{
		private:
			// neuron number on different layers
			unsigned int _hidden_neuron_count, _color_neuron_count;
			// number of parameters for optimization
			unsigned int _param_count;
			// network weights
			arma::mat _hidden_weights, _chemical_weights, _color_weights;
			// execution of the chemical and color computation
			void forward_pass(
				common_types::real_vector& input,
				common_types::real_vector& inner, 
				common_types::real_vector& external,
				common_types::real_vector& color) const;
		public:
			feedforward_ann_controller(lattice_settings const& settings);
			virtual common_types::real_vector get_params() const override;
			virtual void set_params(common_types::real_vector const& p) override;
			virtual void set_next_state(phenotypes::lattice_cell& cell) const override;
		};

		/**
			Conversion for the optimizer, inserts all the elements
			if the given matrix row by row into the output vector using the iterator
		*/
		void matrix_into_vector(arma::mat const& matrix, 
			common_types::real_vector::iterator& itIndex);

		/**
			Conversion for the optimizer, copies all the elements
			of the given vector based on the iterator into the output matrix row by row
		*/
		void vector_into_matrix(arma::mat& matrix, 
			common_types::real_vector::const_iterator& vec);
	}
}

#endif