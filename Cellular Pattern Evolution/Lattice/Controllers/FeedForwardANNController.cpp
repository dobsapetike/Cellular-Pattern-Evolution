#include "../Headers/Controllers/FeedForwardANNController.h"
#include <iostream>

namespace lattice
{
	namespace controllers
	{
		feedforward_ann_controller::feedforward_ann_controller(lattice_settings const& settings)
			: state_controller(settings.stateSettings)
		{
			auto init_config = *settings.controller;
			_hidden_neuron_count = atoi(init_config.FirstChildElement("HiddenNeuronCount")->GetText());
			
			_color_neuron_count = common_types::parse_color_type(
				init_config.FirstChildElement("ColorType")->GetText()) == common_types::rgb ? 3 : 1;

			// initialize weight matrices
			// TODO be able to read weight values from config

			arma::arma_rng::set_seed_random();
			_hidden_weights = arma::mat(
				// row number -> internals + externals of (4) neighbours + bias 
				state_params.internal_chemical_count + 4 * state_params.external_chemical_count + 1,
				// column number -> hidden units
				_hidden_neuron_count,
				arma::fill::randu
			);
			_chemical_weights = arma::mat(
				_hidden_neuron_count + 1,
				// column number -> the new chemicals of the cell
				state_params.internal_chemical_count + state_params.external_chemical_count,
				arma::fill::randu
			);
			_color_weights = arma::mat(
				state_params.internal_chemical_count + state_params.external_chemical_count + 1,
				// the resulting color
				_color_neuron_count,	
				arma::fill::randu
			);

			// the number of parameters of optimization is the number of weights
			_param_count = _hidden_weights.n_elem +
				_chemical_weights.n_elem + _color_weights.n_elem;
		}

		common_types::real_vector feedforward_ann_controller::get_params() const
		{
			common_types::real_vector params(_param_count);
			
			matrix_into_vector(_hidden_weights, params.begin());
			unsigned int index(_hidden_weights.n_elem);
			matrix_into_vector(_chemical_weights, params.begin() + index);
			index += _chemical_weights.n_elem;
			matrix_into_vector(_color_weights, params.begin() + index);

			return params;
		}

		/**
			Sets new parameters by filling the weight matrices  
		*/
		void feedforward_ann_controller::set_params(common_types::real_vector const& p)
		{
			if (p.size() != _param_count)
				throw invalid_argument("Param number mismatch");

			vector_into_matrix(_hidden_weights, p.begin());
			vector_into_matrix(_chemical_weights, p.begin() + _hidden_weights.n_elem);
			vector_into_matrix(_color_weights, p.begin() + 
				_hidden_weights.n_elem + _chemical_weights.n_elem);
		}

		/**
			Execution of the chemical and color computation.
			Each of them is received as an output parameter.
		*/
		void feedforward_ann_controller::forward_pass (
			common_types::real_vector& input,
			common_types::real_vector& inner,
			common_types::real_vector& external,
			common_types::real_vector& color) const
		{
			// compute chemicals
			arma::mat matInput(input);
			arma::mat hidden = matInput.t() * _hidden_weights;
			hidden.set_size(1, hidden.n_cols + 1);
			hidden(0, hidden.n_cols - 1) = -1;			// append bias	
			arma::mat output = hidden * _chemical_weights;

			// extract chemical values
			auto row = arma::conv_to<vector<double>>::from(output.row(0));
			inner = common_types::real_vector(row.begin(), row.begin() + state_params.external_chemical_count);
			external = common_types::real_vector(row.begin() + state_params.external_chemical_count, row.end());

			output.set_size(1, output.n_cols + 1);
			output(0, output.n_cols - 1) = -1;
			// now, compute the color
			arma::mat colorLayer = output * _color_weights;
			color = arma::conv_to<vector<double>>::from(colorLayer.row(0));
		}

		void feedforward_ann_controller::set_next_state(phenotypes::lattice_cell& cell) const
		{
			// first, create the neural network input
			common_types::real_vector annInp;
			
			// insert internals
			auto internals = cell.get_state().internal_chemicals;
			annInp.insert(annInp.end(), internals.begin(), internals.end());

			// insert neighbour externals
			auto nbh = cell.get_neighbours();
			// TODO apply blur
			for (unsigned int i = common_types::upper; i <= common_types::lower; ++i)
			{
				common_types::direction dir = static_cast<common_types::direction>(i);
				if (nbh[dir].size() == 0)
				{
					// fill with zeroes values
					auto zeros = vector<double>(state_params.external_chemical_count, 0.8);
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
			annInp.push_back((double) BIAS);

			// compute and set new state
			common_types::real_vector newInternals, newExternals, newColor;
			forward_pass(annInp, newInternals, newExternals, newColor);

			state newState = {
				newInternals,
				newExternals,
			};
			cell.set_state(newState);
		}

		/**
			Conversion for the optimizer, inserts all the elements
			if the given matrix row by row into the output vector using the iterator
		*/
		void matrix_into_vector(arma::mat const& matrix, 
			common_types::real_vector::iterator& iterator)
		{
			for (unsigned int row = 0; row < matrix.n_rows; ++row)
			{
				for (unsigned int col = 0; col < matrix.n_cols; ++col)
				{
					*iterator++ = matrix(row, col);
				}
			}
		}

		/**
			Conversion for the optimizer, copies all the elements
			of the given vector based on the iterator into the output matrix row by row
		*/
		void vector_into_matrix(arma::mat& matrix, 
			common_types::real_vector::const_iterator& iterator)
		{
			for (unsigned int row = 0; row < matrix.n_rows; ++row)
			{
				for (unsigned int col = 0; col < matrix.n_cols; ++col)
				{
					matrix(row, col) = *iterator++;
				}
			}
		}
	}
}