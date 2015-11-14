#include "../Headers/Controllers/FeedForwardANNController.h"

namespace lattice
{
	namespace controllers
	{
		feedforward_ann_controller::feedforward_ann_controller(lattice_settings const& settings)
			: state_controller(settings.stateSettings)
		{
			// find out the number of neurons on different layers
			auto init_config = *settings.controller;
			_hidden_neuron_count = atoi(init_config.FirstChildElement("HiddenNeuronCount")->GetText());
			_chemical_neuron_count = 
				// internal + external chemicals		
				state_params.internal_chemical_count + state_params.external_chemical_count;

			_color_neuron_count = parse_color_type(
				init_config.FirstChildElement("ColorType")->GetText()) == color_type::rgb ? 3 : 1;


			_hidden_weight_count = 
				// row number -> intput: internals + externals of (4) neighbours + bias 
				(state_params.internal_chemical_count + 4 * state_params.external_chemical_count + 1)
				// column number -> hidden units
				* _hidden_neuron_count;
			_chemical_weight_count =
				(_hidden_neuron_count + 1)	// +bias
				// column number -> the new chemicals of the cell
				* _chemical_neuron_count;
			_color_weight_count =
				(_chemical_neuron_count + 1) // +bias
				// column number -> the resulting color
				* _color_neuron_count;

			// the number of parameters of optimization is the number of weights
			param_count = _hidden_weight_count + _chemical_weight_count + _color_weight_count;

			// randonly initialize weights
			// TODO be able to read weight values from config
			_params = real_vector(param_count);
			for (unsigned int i = 0; i < _params.size(); ++i)
			{
				_params[i] = static_cast<double>(rand()) / RAND_MAX;
			}
		}

		real_vector feedforward_ann_controller::get_params() const
		{
			return _params;
		}

		/**
			Sets new parameters by filling the weight matrices  
		*/
		void feedforward_ann_controller::set_params(real_vector const& p) const
		{
			if (p.size() != param_count)
				throw invalid_argument("Param number mismatch");

			_params = p;
		}

		/**
			Used in the forward pass, linear combination (output) of a layer (input) 
			with the respective weights (given as the weightIndex)
		*/
		void feedforward_ann_controller::execute_linear_combination(
			unsigned int resSize, unsigned int& weightIndex,
			real_vector const& input, real_vector& output) const
		{
			output = real_vector(resSize);
			for (unsigned int i = 0; i < resSize; ++i)
			{
				double linComb = .0;
				for (unsigned int j = 0; j < input.size(); ++j)
				{
					linComb += _params[weightIndex++] * input[j];
				}
				// use hyperbolic tangent as activation
				output[i] = tanh(linComb);
			}
		}

		/**
			Execution of the chemical and color computation.
			Each of them is received as an output parameter.
		*/
		void feedforward_ann_controller::forward_pass (
			real_vector& input,
			real_vector& inner,
			real_vector& external,
			real_vector& color) const
		{
			unsigned int weightIndex = 0;

			// compute hidden neuron values
			real_vector hidden;
			execute_linear_combination(_hidden_neuron_count, weightIndex, input, hidden);
			hidden.push_back(BIAS);	// append bias

			// compute output values
			real_vector output;
			execute_linear_combination(_chemical_neuron_count, weightIndex, hidden, output);

			// extract chemical values
			inner = real_vector(output.begin(), output.begin() + state_params.external_chemical_count);
			external = real_vector(output.begin() + state_params.external_chemical_count, output.end());

			// now, compute the color
			output.push_back(BIAS);
			execute_linear_combination(_color_neuron_count, weightIndex, output, color);

			// compute color level
			for (unsigned int i = 0; i < color.size(); ++i)
			{
				color[i] = (1.0 + color[i]) / 2.0;
			}
		}

		void feedforward_ann_controller::set_next_state(phenotypes::lattice_cell& cell) const
		{
			// first, create the neural network input
			real_vector annInp;
			
			// insert internals
			auto internals = cell.get_state().internal_chemicals;
			annInp.insert(annInp.end(), internals.begin(), internals.end());

			// insert neighbour externals
			auto nbh = cell.get_neighbours();
			// TODO apply blur
			for (unsigned int i = upper; i <= lower; ++i)
			{
				direction dir = static_cast<direction>(i);
				if (nbh[dir].size() == 0)
				{
					// fill with zeroes
					auto zeros = vector<double>(state_params.external_chemical_count, 0.0);
					annInp.insert(annInp.end(), zeros.begin(), zeros.end());
					continue;
				}

				// insert the mean of all the neighbours - TODO use compatibility
				for (unsigned int extChIndex = 0; extChIndex < state_params.external_chemical_count; ++extChIndex)
				{
					double ecSum = 0.0;
					for (auto neighbour : nbh[dir])
					{
						ecSum += (*neighbour).get_state().external_chemicals[extChIndex];
					}
					annInp.push_back(ecSum / nbh[dir].size());
				}
			}

			// also append a bias unit
			annInp.push_back(static_cast<double>(BIAS));

			// compute and set new state
			real_vector newInternals, newExternals, newColor;
			forward_pass(annInp, newInternals, newExternals, newColor);

			state newState = {
				newInternals,
				newExternals,
				color_level_to_rgb(newColor)
			};

			cell.next_candidate(newState);
		}
	}
}