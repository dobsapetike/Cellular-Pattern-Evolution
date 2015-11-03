#include <tinyxml/tinyxml.h>
#include "Headers/ExperimentCollection.h"

namespace task
{
	/**
		Constructor receives a file a reads the experiments
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
			exp->controller_file = experimentElem->FirstChildElement("Controller")->GetText();
			exp->lattice_file = experimentElem->FirstChildElement("Lattice")->GetText();
			exp->objective_file = experimentElem->FirstChildElement("Objective")->GetText();
			exp->optimizer_file = experimentElem->FirstChildElement("Optimizer")->GetText();
			exp->stopcriterion_file = experimentElem->FirstChildElement("StopCriterion")->GetText();
			_experiments.push_back(move(exp));

			experimentElem = experimentElem->NextSibling();
		}
	}

	/**
		Given id, returns a pointer to the experiment - as a function or bracket operator
	*/
	std::shared_ptr<experiment> const& experiments::get_experiment(unsigned int id) const
	{
		for (auto& expptr : _experiments)
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
}