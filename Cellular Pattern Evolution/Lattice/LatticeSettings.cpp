#include "Headers\LatticeSettings.h"
#include <iostream>

namespace lattice
{
	/**
		Loads the lattice settings from the given XML file
	*/
	std::unique_ptr<lattice_settings> load_settings(string const& filepath)
	{
		TiXmlDocument doc(filepath.c_str());
		if (!doc.LoadFile())
		{
			throw std::invalid_argument("Could not parse file: " + filepath);
		}
		
		unique_ptr<lattice_settings> settings = std::make_unique<lattice_settings>();
		auto settingsElem = doc.FirstChild("LatticeSettings");

		settings->width = atoi(settingsElem->FirstChildElement("LatticeWidth")->GetText());
		settings->height = atoi(settingsElem->FirstChildElement("LatticeHeight")->GetText());

		settings->cellType = phenotypes::parse_cell_type(
			settingsElem->FirstChildElement("CellType")->GetText());
		settings->target = settingsElem->FirstChildElement("TargetPattern")->GetText();
		settings->init_pattern = settingsElem->FirstChildElement("InitPattern")->GetText();

		settings->controller = unique_ptr<TiXmlElement>(
			settingsElem->FirstChildElement("Controller")->Clone()->ToElement());
		settings->stop_criterion = unique_ptr<TiXmlElement>(
			settingsElem->FirstChildElement("StopCriterion")->Clone()->ToElement());
		settings->phenotype_settings = unique_ptr<TiXmlElement>(
			settingsElem->FirstChildElement("PhenotypeSettings")->Clone()->ToElement());

		settings->stateSettings.internal_chemical_count = atoi(settingsElem->
			FirstChildElement("State")->
			FirstChildElement("InternalCount")->GetText());
		settings->stateSettings.external_chemical_count = atoi(settingsElem->
			FirstChildElement("State")->
			FirstChildElement("ExternalCount")->GetText());
		settings->stateSettings.init_val = (double) atof(settingsElem->
			FirstChildElement("State")->
			FirstChildElement("InitialValues")->GetText());

		return settings;
	}
}