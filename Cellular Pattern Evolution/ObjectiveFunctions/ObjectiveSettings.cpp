#include "Headers/ObjectiveSettings.h"

namespace objective_functions
{
	/**
		Loads the objective settings from the given XML file
	*/
	std::unique_ptr<objective_settings> load_settings(std::string const& filepath)
	{
		TiXmlDocument doc(filepath.c_str());
		if (!doc.LoadFile())
			throw std::invalid_argument("Could not parse file: " + filepath);

		std::unique_ptr<objective_settings> settings = std::make_unique<objective_settings>();
		auto settingsElem = doc.FirstChild("Settings")->FirstChild("ObjectiveSettings");

		settings->min_value = atof(settingsElem->FirstChildElement("MinValue")->GetText());
		settings->max_value = atof(settingsElem->FirstChildElement("MaxValue")->GetText());

		settings->objfunc_settings = std::unique_ptr<TiXmlElement>(
			settingsElem->FirstChildElement("ObjectiveFunctions")->Clone()->ToElement());

		return settings;
	}
}