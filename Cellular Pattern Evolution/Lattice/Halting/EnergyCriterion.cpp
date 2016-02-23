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
			threshold = atof(settings.stop_criterion->FirstChildElement("Threshold")->GetText());
			window_size = atoi(settings.stop_criterion->FirstChildElement("WindowSize")->GetText());
			step_limit = atoi(settings.stop_criterion->FirstChildElement("StepLimit")->GetText());
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
			//energy += cell.get_state().color.r;
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
			
			energy_history.push_back(energy);
			history_sum += energy;
			if (energy_history.size() < window_size)
				// check criterion only after you have a complete time window to use
				return false;

			// remove the oldest when window has been exceeded
			if (energy_history.size() > window_size)
			{
				history_sum -= energy_history[0];
				energy_history.erase(energy_history.begin());
			}

			// compute the average of the history
			avg = history_sum / window_size;

			// and the standard deviation of the energy
			for (double en : energy_history)
			{
				double diff = en - avg;
				stdev += diff * diff;
			}
			stdev = sqrt(stdev / window_size);

			bool shouldStop = stdev < threshold
				|| lattice.get_statistics().sim_eval_count >= step_limit;
			if (shouldStop)
			{
				/*cout << "Stopping after steps: " << lattice.get_statistics().eval_count << 
					" ( " << stdev << " )" << endl;*/
			}
			return shouldStop;
		}
	}
}