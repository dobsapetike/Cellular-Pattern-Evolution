#ifndef OBJECTIVE_SETTINGS_H
#define OBJECTIVE_SETTINGS_H

#include <string>
#include <tinyxml/tinyxml.h>
#include <memory>

namespace objective_functions
{
	/**
		Structure describing the mulit-objective function settings
	*/
	struct objective_settings
	{
		// result contraints
		double min_value;
		double max_value;

		// xml element of the objective functions and their settings
		std::unique_ptr<TiXmlElement> objfunc_settings;
	};

	/**
		Loads the objective settings from the given XML file
	*/
	std::unique_ptr<objective_settings> load_settings(std::string const& filepath);
}

#endif