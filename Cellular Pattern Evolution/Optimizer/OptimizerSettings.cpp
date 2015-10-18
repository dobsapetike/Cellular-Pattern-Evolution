#include "Headers/OptimizerSettings.h"

namespace optimizer
{
	/**
		Loads the optimizer settings from the given XML file
	*/
	std::unique_ptr<optimizer_settings> load_settings(std::string const& filepath)
	{
		TiXmlDocument doc(filepath.c_str());
		if (!doc.LoadFile())
			throw std::invalid_argument("Could not parse file: " + filepath);

		std::unique_ptr<optimizer_settings> settings = std::make_unique<optimizer_settings>();
		auto settingsElem = doc.FirstChild("Settings")->FirstChild("OptimizerSettings");

		settings->algorithm = settingsElem->FirstChildElement("Algorithm")->GetText();
		settings->params = std::unique_ptr<TiXmlElement>(
			settingsElem->FirstChildElement("Params")->Clone()->ToElement());

		return settings;
	}
}