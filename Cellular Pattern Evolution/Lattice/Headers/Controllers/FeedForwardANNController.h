#ifndef FEED_FORWARD_ANN_CONTROLLER_H
#define FEED_FORWARD_ANN_CONTROLLER_H

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
			unsigned int hidden_neuron_count, chemical_neuron_count, output_neuron_count;
			// network weights and number of weights and different layers
			mutable real_vector params;
			unsigned int hidden_weight_count, chemical_weight_count, output_weight_count;
			// execution of the chemical and color computation
			void forward_pass(
				real_vector const& input,
				real_vector& inner, 
				real_vector& external,
				real_vector& color,
				real_vector& action) const;
			// used in the forward pass, linear combination of a layer with the respective weights
			void execute_linear_combination(
				unsigned int resSize, unsigned int& weightIndex,
				real_vector const& input, real_vector& output) const;

			// For a cell neighbourhood, compute the mean of externals in every direction 
			vector<real_vector> feedforward_ann_controller::compute_neighbour_external_means(
				phenotypes::neighbourhood& nbh, phenotypes::neighbourhood_compatibility& nbc) const;
			// Given a lattice cell, returns it's blurred externals using gaussian blur with radius of one
			real_vector feedforward_ann_controller::compute_blurred_neighbour_external(
				phenotypes::lattice_cell& cell) const;
			// Computes a vector containing the blurred externals of neighbours in all 4 directions
			vector<real_vector> feedforward_ann_controller::compute_externals(
				phenotypes::lattice_cell& cell) const;
		public:
			feedforward_ann_controller(lattice_settings const& settings);

			// interface implementation

			real_vector get_params() const override;
			void set_params(real_vector const& p) const override;
			void set_next_state(phenotypes::lattice_cell& cell) const override;
		};
	}
}

#endif