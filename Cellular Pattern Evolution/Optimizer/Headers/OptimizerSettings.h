#ifndef OPTIMIZER_SETTINGS_H
#define OPTIMIZER_SETTINGS_H

#include <string>
#include <tinyxml/tinyxml.h>
#include <memory>

namespace optimizer
{
	/**
		Structure describing the optimizer settings
	*/
	struct optimizer_settings
	{
		std::string algorithm;

		// xml element of the algorithm parameters
		std::unique_ptr<TiXmlElement> params;
	};

	/**
		Loads the optimizer settings from the given XML file
	*/
	std::unique_ptr<optimizer_settings> load_settings(std::string const& filepath);
}

#endif