#include "../Headers/Controllers/FeedForwardANNController.h"
#include <numeric>

namespace lattice
{
	namespace controllers
	{
		feedforward_ann_controller::feedforward_ann_controller(lattice_settings const& settings)
			: state_controller(settings.stateSettings)
		{
			auto& init_config = *settings.controller;
			color_type = parse_color_type(init_config.FirstChildElement("ColorType")->GetText());
			use_fixed_intervals = atoi(init_config.FirstChildElement("FixedIntervals")->GetText());
			
			// find out the number of neurons on different layers
			hidden_neuron_count = atoi(init_config.FirstChildElement("HiddenNeuronCount")->GetText());
			chemical_neuron_count = 
				// internal + external chemicals		
				state_params.internal_chemical_count + state_params.external_chemical_count;

			output_neuron_count = 
				4 +		// merge/split action and interval values 
				(color_type == color_type::rgb ? 3 : 1);	// neuron for each component 


			hidden_weight_count = 
				// row number -> intput: internals + externals of (4) neighbours + bias 
				(state_params.internal_chemical_count + 4 * state_params.external_chemical_count + 1)
				// column number -> hidden units
				* hidden_neuron_count;
			chemical_weight_count =
				(hidden_neuron_count + 1)	// +bias
				// column number -> the new chemicals of the cell
				* chemical_neuron_count;
			output_weight_count =
				(chemical_neuron_count + 1) // +bias
				// column number -> the resulting color
				* output_neuron_count;

			// the number of parameters of optimization is the number of weights
			param_count = hidden_weight_count + chemical_weight_count + output_weight_count;

			// randomly initialize weights
			params = real_vector(param_count);
			// try to read weight values from config - place random if none exist
			bool avail;
			double value;
			TiXmlElement* paramElem = init_config.FirstChildElement("Params");
			stringstream sts(paramElem->FirstChild() ? paramElem->GetText() : "");
			for (unsigned int i = 0; i < params.size(); ++i)
			{
				avail = sts.rdbuf()->in_avail() > 0;
				sts >> value;
				params[i] = avail ? value : static_cast<double>(rand()) / RAND_MAX;
			}
		}

		real_vector feedforward_ann_controller::get_params() const
		{
			return params;
		}

		/**
			Sets new parameters by filling the weight matrices  
		*/
		void feedforward_ann_controller::set_params(real_vector const& p) const
		{
			if (p.size() != param_count)
				throw invalid_argument("Param number mismatch");

			params = p;
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
					linComb += params[weightIndex++] * input[j];
				}
				// use hyperbolic tangent for activation
				output[i] = tanh(linComb);
			}
		}

		/**
			Execution of the chemical and color computation.
			Each of them is received as an output parameter.
		*/
		void feedforward_ann_controller::forward_pass (
			real_vector const& input,
			real_vector& inner,
			real_vector& external,
			real_vector& color,
			real_vector& action) const
		{
			unsigned int weightIndex = 0;

			// compute hidden neuron values
			real_vector hidden;
			execute_linear_combination(hidden_neuron_count, weightIndex, input, hidden);
			hidden.push_back(BIAS);	// append bias

			// compute output values
			real_vector output;
			execute_linear_combination(chemical_neuron_count, weightIndex, hidden, output);

			// extract chemical values
			inner = real_vector(output.begin(), output.begin() + state_params.internal_chemical_count);
			external = real_vector(output.begin() + state_params.internal_chemical_count, output.end());

			// now, compute the color
			output.push_back(BIAS);
			execute_linear_combination(output_neuron_count, weightIndex, output, color);

			// compute color level and merge/split action
			action = real_vector(4);
			// interval values
			for (unsigned int i = 3; i; --i)
			{
				action[i] = (1.0 + color.back()) / 2.0;
				color.pop_back();
			}
			// action value
			action[0] = color.back();
			color.pop_back();
			// color
			for (unsigned int i = 0; i < color.size(); ++i)
			{
				color[i] = (1.0 + color[i]) / 2.0;
			}
		}

		/**
			For a cell neighbourhood, compute the mean of externals in every direction 
		*/
		vector<real_vector> feedforward_ann_controller::compute_neighbour_external_means(
			phenotypes::neighbourhood& nbh, phenotypes::neighbourhood_compatibility& nbc) const
		{
			vector<real_vector> res;
			for (unsigned int i = upper; i <= lower; ++i)
			{
				direction d = static_cast<direction>(i);
				real_vector vec = vector<double>(state_params.external_chemical_count, 0.0);
				if (nbh[d].size() > 0)
				{
					// compute the mean of all neighbours in the given direction
					for (unsigned int extChIndex = 0; extChIndex < state_params.external_chemical_count; ++extChIndex)
					{
						double ecSum = 0.0;
						for (auto& neighbour : nbh[d])
						{
							double comp = nbc[d][neighbour];
							ecSum += (*neighbour).get_state().external_chemicals[extChIndex] * comp;
						}
						vec[extChIndex] = ecSum / nbh[d].size();
					}
				}
				res.push_back(vec);
			}
			return res;
		}

		/**
			Given a lattice cell, returns it's blurred externals using gaussian blur with radius of one
		*/
		void feedforward_ann_controller::compute_blurred_neighbour_external(
			phenotypes::lattice_cell& cell) const
		{
			auto nbh = cell.get_neighbours();
			auto nbc = cell.get_neighbour_compatibility();
			auto neighExts = compute_neighbour_external_means(nbh, nbc);

			real_vector vec = vector<double>(state_params.external_chemical_count, 0.0);
			auto curExt = cell.get_state().external_chemicals;
			for (unsigned int extChIndex = 0; extChIndex < state_params.external_chemical_count; ++extChIndex)
			{
				vec[extChIndex] = curExt[extChIndex] / 2.0
				   + neighExts[lower][extChIndex] / 8.0 
				   + neighExts[direction::left][extChIndex] / 8.0 + neighExts[direction::right][extChIndex] / 8.0
				   + neighExts[upper][extChIndex] / 8.0;
			}
			cell.set_blurred_externals(vec);
		}

		/**
			Computes a vector containing the blurred externals of neighbours in
			all 4 directions
		*/
		vector<real_vector> feedforward_ann_controller::compute_externals(
			phenotypes::lattice_cell& cell) const
		{
			vector<real_vector> res;
			auto nbh = cell.get_neighbours();
			for (unsigned int i = upper; i <= lower; ++i)
			{
				direction d = static_cast<direction>(i);
				real_vector vec = vector<double>(state_params.external_chemical_count, 0.0);

				if (nbh[d].size() > 0)
				{
					vector<real_vector> blurredNeighExts;
					for (auto neighbour : nbh[d])
					{
						// blur each neighbour's externals
						blurredNeighExts.push_back(neighbour->get_blurred_externals());
					}
					// compute the mean of blurred neighbour externals in the given direction
					for (unsigned int extChIndex = 0; extChIndex < state_params.external_chemical_count; ++extChIndex)
					{
						double ecSum = 0.0;
						for (auto& blNeigh : blurredNeighExts)
						{
							ecSum += blNeigh[extChIndex];
						}
						vec[extChIndex] = ecSum / nbh[d].size();
					}
				}
				res.push_back(vec);
			}
			return res;
		}

		action feedforward_ann_controller::choose_action(real_vector& values) const
		{
			if (use_fixed_intervals) 
			{
				return values[0] < -1.0 + 2.0 / 3 ? merge :
					   values[0] >  1.0 - 2.0 / 3 ? split :
													nothing;
			}

			// create prob distribution and splice the interval
			double total = accumulate(next(values.begin()), values.end(), 0.0);
			for (unsigned int i = 1; i < values.size(); ++i) values[i] *= 2.0 / total;
			// and splice the interval
			return values[0] < -1.0 + values[1] ? merge :
				   values[0] > 1.0 - values[2]  ? split : 
												  nothing;
		}

		void feedforward_ann_controller::set_next_state(phenotypes::lattice_cell& cell) const
		{
			// first, create the neural network input
			real_vector annInp;
			
			// insert internals
			auto internals = cell.get_state().internal_chemicals;
			annInp.insert(annInp.end(), internals.begin(), internals.end());

			// insert neighbour externals
			auto externals = compute_externals(cell);
			for (auto& ext : externals)
			{
				annInp.insert(annInp.end(), ext.begin(), ext.end());
			}

			// also append a bias unit
			annInp.push_back(static_cast<double>(BIAS));

			// compute and set new state
			real_vector newInternals, newExternals, newColor, msAction;
			forward_pass(annInp, newInternals, newExternals, newColor, msAction);

			state newState = {
				newInternals,
				newExternals,
				color_level_to_rgb(newColor),
				choose_action(msAction)
			};

			// set new state as a candidate
			cell.next_candidate(newState);
		}
	}
}