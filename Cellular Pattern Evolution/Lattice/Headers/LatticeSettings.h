#ifndef LATTICE_SETTINGS_H
#define LATTICE_SETTINGS_H

#include "Phenotypes\Cell\CellType.h"
#include "State.h"
#include "TargetPatternSvg.h"
#include <string>
#include <tinyxml/tinyxml.h>
#include <memory>

using namespace std;

namespace lattice
{
	/**
		Structure containing all the necessary information from which
		the respective automaton can be constructed
		*/
	struct lattice_settings
	{
		// spacial spread of the CA
		unsigned int width, height;

		phenotypes::cell_type cellType;

		state_settings stateSettings;

		// a string describing the initial structure of the CA
		// the form depends on the cell type
		string init_pattern;

		// field describing the target pattern
		unique_ptr<target_pattern_svg> target;

		// !!! settings with properties that are not fixed are stored 
		// as xml elements for the respective class to parse 
		unique_ptr<TiXmlElement> stop_criterion;
		unique_ptr<TiXmlElement> controller;
		unique_ptr<TiXmlElement> phenotype_settings;
	};

	/**
		Loads the lattice settings from the given XML files
		*/
	unique_ptr<lattice_settings> load_settings(string const& lattice_config,
		string const& controller_config, string const& stopcrit_config);
}

#endif