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
			unsigned int _hidden_neuron_count, _chemical_neuron_count, _color_neuron_count;
			// network weights and number of weights and different layers
			mutable real_vector _params;
			unsigned int _hidden_weight_count, _chemical_weight_count, _color_weight_count;
			// execution of the chemical and color computation
			void forward_pass(
				real_vector& input,
				real_vector& inner, 
				real_vector& external,
				real_vector& color) const;
			// used in the forward pass, linear combination of a layer with the respective weights
			void execute_linear_combination(
				unsigned int resSize, unsigned int& weightIndex,
				real_vector const& input, real_vector& output) const;
		public:
			feedforward_ann_controller(lattice_settings const& settings);
			virtual real_vector get_params() const override;
			virtual void set_params(real_vector const& p) const override;
			virtual void set_next_state(phenotypes::lattice_cell& cell) const override;
		};
	}
}

#endif