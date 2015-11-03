#include "Headers\LatticeSettings.h"

namespace lattice
{
	/**
		Loads the lattice settings from the given XML files
	*/
	unique_ptr<lattice_settings> load_settings(string const& lattice_config,
		string const& controller_config, string const& stopcrit_config)
	{
		TiXmlDocument latDoc(lattice_config.c_str());
		if (!latDoc.LoadFile())
			throw invalid_argument("Could not parse file: " + lattice_config);
		
		unique_ptr<lattice_settings> settings = make_unique<lattice_settings>();
		auto settingsElem = latDoc.FirstChild("LatticeSettings");

		settings->width = atoi(settingsElem->FirstChildElement("LatticeWidth")->GetText());
		settings->height = atoi(settingsElem->FirstChildElement("LatticeHeight")->GetText());

		settings->cellType = phenotypes::parse_cell_type(
			settingsElem->FirstChildElement("CellType")->GetText());
		settings->init_pattern = settingsElem->FirstChildElement("InitPattern")->GetText();

		string target_path = settingsElem->FirstChildElement("TargetPattern")->GetText();
		settings->target = make_unique<target_pattern>(settings->width, settings->height, target_path);
	
		settings->phenotype_settings = unique_ptr<TiXmlElement>(
			settingsElem->FirstChildElement("PhenotypeSettings")->Clone()->ToElement());

		settings->stateSettings.internal_chemical_count = atoi(settingsElem->
			FirstChildElement("State")->
			FirstChildElement("InternalCount")->GetText());
		settings->stateSettings.external_chemical_count = atoi(settingsElem->
			FirstChildElement("State")->
			FirstChildElement("ExternalCount")->GetText());
		settings->stateSettings.init_val = static_cast<double>(atof(settingsElem->
			FirstChildElement("State")->
			FirstChildElement("InitialValues")->GetText()));

		TiXmlDocument contrDoc(controller_config.c_str());
		if (!contrDoc.LoadFile())
			throw invalid_argument("Could not parse file: " + controller_config);
		settings->controller = unique_ptr<TiXmlElement>(
			contrDoc.FirstChild("Controller")->Clone()->ToElement());

		TiXmlDocument stCritDoc(stopcrit_config.c_str());
		if (!stCritDoc.LoadFile())
			throw invalid_argument("Could not parse file: " + stopcrit_config);
		settings->stop_criterion = unique_ptr<TiXmlElement>(
			stCritDoc.FirstChildElement("StopCriterion")->Clone()->ToElement());

		return settings;
	}
}