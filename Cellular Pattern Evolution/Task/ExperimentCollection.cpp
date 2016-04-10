#include <tinyxml/tinyxml.h>
#include "Headers/ExperimentCollection.h"

namespace task
{
	/**
		Constructor receives a file and reads the experiments
	*/
	experiments::experiments(std::string filepath)
	{
		TiXmlDocument doc(filepath.c_str());
		if (!doc.LoadFile()) 
			throw std::invalid_argument("Could not parse file: " + filepath);

		auto experimentElem = doc.FirstChild("Experiments")->FirstChild();
		if (experimentElem == nullptr)
			throw std::invalid_argument("No experiment specified!");

		while (experimentElem != nullptr)
		{
			if (strcmpi(experimentElem->Value(), "Experiment"))
				throw std::invalid_argument("Experiment has to be specified as type \"Experiment\"!");

			auto exp = std::make_shared<experiment>();
			exp->id = atoi(experimentElem->FirstChildElement("Id")->GetText());
			exp->name = experimentElem->FirstChildElement("Name")->GetText();
			exp->run_count = atoi(experimentElem->FirstChildElement("RunCount")->GetText());
			exp->controller_file = experimentElem->FirstChildElement("Controller")->GetText();
			exp->lattice_file = experimentElem->FirstChildElement("Lattice")->GetText();
			exp->objective_file = experimentElem->FirstChildElement("Objective")->GetText();
			exp->optimizer_file = experimentElem->FirstChildElement("Optimizer")->GetText();
			exp->stopcriterion_file = experimentElem->FirstChildElement("StopCriterion")->GetText();
			exp->video_fps = atoi(experimentElem->FirstChildElement("VideoFPS")->GetText());
			exp->generate_video = atoi(experimentElem->FirstChildElement("GenerateVideo")->GetText());
			exp->simulate_only = atoi(experimentElem->FirstChildElement("SimulateOnly")->GetText());
			exp->observed_runs = atoi(experimentElem->FirstChildElement("ObservedRuns")->GetText());
			exp->plot = atoi(experimentElem->FirstChildElement("Plot")->GetText());
			exp->rich_video = atoi(experimentElem->FirstChildElement("RichVideo")->GetText());
			experiments_list.push_back(move(exp));

			experimentElem = experimentElem->NextSibling();
		}
	}

	/**
		Given id, returns a pointer to the experiment - as a function or bracket operator
	*/
	std::shared_ptr<experiment> const& experiments::get_experiment(unsigned int id) const
	{
		for (auto& expptr : experiments_list)
		{
			if (expptr->id == id) return expptr;
		}

		static std::shared_ptr<experiment> not_found;
		return not_found;
	}

	std::shared_ptr<experiment> const& experiments::operator[](unsigned int id) const
	{
		return get_experiment(id);
	}

	/**
		Getter by index
	*/
	std::shared_ptr<experiment> const& experiments::at(unsigned int index) const
	{
		if (index < 0 || index > experiment_count() - 1)
			throw std::out_of_range("Experiment index is out of range!");
		return experiments_list[index];
	}
}