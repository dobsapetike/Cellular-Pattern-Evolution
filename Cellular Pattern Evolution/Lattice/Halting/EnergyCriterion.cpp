#include "../Headers/Lattice.h"
#include "../Headers/Halting/EnergyCriterion.h"

namespace lattice
{
	namespace halting
	{
		/**
			Reads settings from config
		*/
		energy_halting::energy_halting(lattice_settings const& settings)
		{
			_threshold = atof(settings.stop_criterion->FirstChildElement("Threshold")->GetText());
			_window_size = atoi(settings.stop_criterion->FirstChildElement("WindowSize")->GetText());
			_step_limit = atoi(settings.stop_criterion->FirstChildElement("StepLimit")->GetText());
		}

		/**
			Computes the energy of a lattice cell
		*/
		double energy_halting::compute_energy(phenotypes::lattice_cell& cell) const
		{
			double energy(0.0);
			for (double chemical : cell.get_state().external_chemicals)
			{
				energy += chemical * chemical;
			}
			for (double chemical : cell.get_state().internal_chemicals)
			{
				energy += chemical * chemical;
			}
			return sqrt(energy);
		}

		/**
			Energy criterion based on Devert
		*/
		bool energy_halting::should_stop(lattice& lattice)
		{
			double energy(0.0), avg(0.0), stdev(0.0);
			
			// compute energy of the organism
			auto cells = lattice.get_phenotype().expose_cells();
			for (auto& cell : cells)
			{
				energy += compute_energy(*cell);
			}
			energy = sqrt(energy);
			
			_energy_history.push_back(energy);
			_history_sum += energy;
			if (_energy_history.size() < _window_size)
				// check criterion only after you have a complete time window to use
				return false;

			// remove the oldest when window has been exceeded
			if (_energy_history.size() > _window_size)
			{
				_history_sum -= _energy_history[0];
				_energy_history.erase(_energy_history.begin());
			}

			// compute the average of the history
			avg = _history_sum / _window_size;

			// and the standard deviation of the energy
			for (double en : _energy_history)
			{
				double diff = en - avg;
				stdev += diff * diff;
			}
			stdev = sqrt(stdev / _window_size);

			return stdev < _threshold 
				|| lattice.get_statistics().eval_count > _step_limit;
		}
	}
}